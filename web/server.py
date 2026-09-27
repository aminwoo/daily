#!/usr/bin/env python3
"""daily web — the exercise set in a browser.

Serves web/index.html and a small JSON API. Tests are run through ./daily,
so the compiler flags, sanitizers, timeouts and .progress are exactly the
ones the CLI uses. Standard library only.

  DAILY_PASSWORD=... python3 web/server.py [--host 0.0.0.0] [--port 8080]

Without DAILY_PASSWORD it refuses to listen on anything but localhost: the
server compiles and runs whatever code it is sent.
"""
import argparse
import hashlib
import hmac
import json
import os
import re
import signal
import subprocess
import sys
import threading
import time
from http.cookies import SimpleCookie
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs

ROOT = Path(__file__).resolve().parent.parent
EX_DIR = ROOT / "exercises"
REF_DIR = ROOT / "reference"
WORK = Path(os.environ.get("DAILY_WORK", ROOT / "work"))
PROGRESS = Path(os.environ.get("DAILY_PROGRESS", ROOT / ".progress"))
INDEX = Path(__file__).resolve().parent / "index.html"
DAILY = str(ROOT / "daily")
# every ./daily we spawn should agree with us about where state lives
os.environ["DAILY_WORK"], os.environ["DAILY_PROGRESS"] = str(WORK), str(PROGRESS)

PASSWORD = os.environ.get("DAILY_PASSWORD", "")
COOKIE = "daily_session"
RUN_TIMEOUT_S = 300  # the harness aborts a single test after 10 s; this is a backstop
MAX_BODY = 1 << 20
MAX_OUTPUT = 2 << 20  # a crash loop or print-in-a-loop can emit gigabytes

# one compile+run at a time: ASan builds are memory-hungry on a small box
run_lock = threading.Lock()

SAMPLE_TEMPLATE = """\
// Sample cases for {ex} — edit freely.  Run with ./daily sample {ex}
// (or <leader>r inside nvim).  Never recorded as a pass.
#include "harness.h"
#include SOLUTION

TEST(example) {{
  CHECK_EQ(1, 1);
}}
"""


def session_token():
    return hmac.new(PASSWORD.encode(), b"daily-session", hashlib.sha256).hexdigest()


def exercises():
    return sorted(p.name for p in EX_DIR.iterdir() if p.is_dir())


def progress():
    out = {}
    if PROGRESS.exists():
        for line in PROGRESS.read_text().splitlines():
            ex, _, date = line.partition("\t")
            if ex:
                out[ex] = date
    return out


def title(ex):
    first = (EX_DIR / ex / "README.md").read_text().split("\n", 1)[0]
    # "# 01 — Disjoint Set Union (Union–Find)" -> "Disjoint Set Union (Union–Find)"
    return re.sub(r"^#\s*\d+\s*[—–-]\s*", "", first).strip() or ex


def work_file(ex):
    f = WORK / f"{ex}.cpp"
    if not f.exists():
        WORK.mkdir(parents=True, exist_ok=True)
        f.write_text((EX_DIR / ex / "skeleton.cpp").read_text())
    return f


def sample_file(ex):
    return WORK / f"{ex}.sample.cpp"


def read_sample(ex):
    f = sample_file(ex)
    return f.read_text() if f.exists() else SAMPLE_TEMPLATE.format(ex=ex)


def today_id(exs):
    # same rotation as `daily today`
    return exs[(int(time.time()) // 86400) % len(exs)]


LOGIN_PAGE = """<!doctype html><html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>daily — sign in</title>
<style>
:root{color-scheme:light dark;--bg:#fafaf9;--fg:#1c1917;--muted:#78716c;--line:#e7e5e4;--accent:#2563eb}
@media (prefers-color-scheme:dark){:root{--bg:#0c0a09;--fg:#e7e5e4;--muted:#a8a29e;--line:#292524;--accent:#60a5fa}}
body{margin:0;min-height:100vh;display:grid;place-items:center;background:var(--bg);color:var(--fg);
font:15px/1.5 ui-sans-serif,system-ui,sans-serif}
form{display:flex;flex-direction:column;gap:12px;width:min(320px,calc(100vw - 32px))}
h1{font:600 22px ui-monospace,monospace;margin:0 0 4px}
input,button{font:inherit;padding:10px 12px;border-radius:8px;border:1px solid var(--line);background:transparent;color:inherit}
button{background:var(--accent);border-color:var(--accent);color:#fff;font-weight:600;cursor:pointer}
.err{color:#dc2626;font-size:13px;margin:0}
</style></head><body><form method="post" action="/login">
<h1>daily</h1>%s<input type="password" name="password" placeholder="Password" autofocus required>
<button>Sign in</button></form></body></html>"""


class Handler(BaseHTTPRequestHandler):
    server_version = "daily"

    # ---- plumbing -------------------------------------------------------

    def log_message(self, fmt, *args):
        sys.stderr.write("%s %s\n" % (self.address_string(), fmt % args))

    def authed(self):
        if not PASSWORD:
            return True
        c = SimpleCookie(self.headers.get("Cookie", ""))
        return COOKIE in c and hmac.compare_digest(c[COOKIE].value, session_token())

    def body(self):
        n = int(self.headers.get("Content-Length") or 0)
        if n > MAX_BODY:
            raise ValueError("body too large")
        return self.rfile.read(n).decode()

    def send(self, code, body, ctype="application/json", headers=()):
        if not isinstance(body, (bytes, str)):
            body = json.dumps(body)
        if isinstance(body, str):
            body = body.encode()
        self.send_response(code)
        self.send_header("Content-Type", ctype + ("; charset=utf-8" if "json" in ctype or "text" in ctype else ""))
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        for k, v in headers:
            self.send_header(k, v)
        self.end_headers()
        self.wfile.write(body)

    def redirect(self, to, headers=()):
        self.send_response(303)
        self.send_header("Location", to)
        self.send_header("Content-Length", "0")
        for k, v in headers:
            self.send_header(k, v)
        self.end_headers()

    def route(self):
        """-> (exercise id or None, action) for /api/ex/<id>[/<action>]"""
        m = re.fullmatch(r"/api/ex/([A-Za-z0-9_]+)(?:/([a-z]+))?", self.path.split("?")[0])
        if not m:
            return None, None
        ex = m.group(1)
        return (ex if ex in exercises() else None), (m.group(2) or "")

    # ---- GET ------------------------------------------------------------

    def do_GET(self):
        path = self.path.split("?")[0]
        if path == "/healthz":
            return self.send(200, "ok", "text/plain")
        if not self.authed():
            if path.startswith("/api/"):
                return self.send(401, {"error": "not signed in"})
            return self.send(200, LOGIN_PAGE % "", "text/html")

        if path == "/":
            return self.send(200, INDEX.read_bytes(), "text/html")
        if path == "/api/exercises":
            exs, prog = exercises(), progress()
            return self.send(200, {
                "exercises": [
                    {"id": ex, "title": title(ex), "passed": prog.get(ex),
                     "started": (WORK / f"{ex}.cpp").exists()}
                    for ex in exs
                ],
                "today": today_id(exs),
                "next": next((ex for ex in exs if ex not in prog), None),
            })

        ex, action = self.route()
        if ex is None:
            return self.send(404, {"error": "not found"})
        if action == "":
            return self.send(200, {
                "id": ex,
                "title": title(ex),
                "readme": (EX_DIR / ex / "README.md").read_text(),
                "code": work_file(ex).read_text(),
                "sample": read_sample(ex),
                "passed": progress().get(ex),
            })
        if action == "reference":
            f = REF_DIR / f"{ex}.cpp"
            return self.send(200, {"code": f.read_text() if f.exists() else ""})
        return self.send(404, {"error": "not found"})

    # ---- POST / PUT -----------------------------------------------------

    def do_POST(self):
        path = self.path.split("?")[0]
        if path == "/login":
            pw = parse_qs(self.body()).get("password", [""])[0]
            if PASSWORD and hmac.compare_digest(pw.encode(), PASSWORD.encode()):
                secure = "; Secure" if self.headers.get("X-Forwarded-Proto") == "https" else ""
                cookie = f"{COOKIE}={session_token()}; Path=/; HttpOnly; SameSite=Strict; Max-Age=31536000{secure}"
                return self.redirect("/", [("Set-Cookie", cookie)])
            time.sleep(1)  # slow down guessing
            return self.send(401, LOGIN_PAGE % '<p class="err">Wrong password.</p>', "text/html")
        if path == "/logout":
            return self.redirect("/", [("Set-Cookie", f"{COOKIE}=; Path=/; Max-Age=0")])

        if not self.authed():
            return self.send(401, {"error": "not signed in"})
        ex, action = self.route()
        if ex is None:
            return self.send(404, {"error": "not found"})

        if action == "reset":
            subprocess.run([DAILY, "reset", ex], cwd=ROOT, capture_output=True, check=True)
            return self.send(200, {"code": work_file(ex).read_text()})
        if action in ("test", "sample"):
            # the request body, if any, is the current editor contents: save it first
            src = self.body()
            if src:
                (work_file(ex) if action == "test" else sample_file(ex)).write_text(src)
            if action == "sample" and not sample_file(ex).exists():
                sample_file(ex).write_text(SAMPLE_TEMPLATE.format(ex=ex))
            return self.stream_run(ex, action)
        return self.send(404, {"error": "not found"})

    def do_PUT(self):
        if not self.authed():
            return self.send(401, {"error": "not signed in"})
        ex, action = self.route()
        if ex is None or action not in ("code", "sample"):
            return self.send(404, {"error": "not found"})
        f = work_file(ex) if action == "code" else sample_file(ex)
        f.write_text(self.body())
        return self.send(200, {"ok": True})

    def stream_run(self, ex, action):
        """Run `daily test|sample <ex>` and stream its output as plain text."""
        self.send_response(200)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Accel-Buffering", "no")
        self.end_headers()  # no Content-Length: the body ends when we close

        work, root = (str(WORK) + "/").encode(), (str(ROOT) + "/").encode()

        def out(b):  # show repo-relative paths, as the CLI does
            self.wfile.write(b.replace(work, b"work/").replace(root, b""))
            self.wfile.flush()

        if not run_lock.acquire(blocking=False):
            out(b"(waiting for another run to finish)\n")
            run_lock.acquire()
        proc = None
        try:
            proc = subprocess.Popen(
                [DAILY, action, ex], cwd=ROOT,
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                start_new_session=True,  # so a kill takes the test binary with it
            )
            timer = threading.Timer(RUN_TIMEOUT_S, lambda: os.killpg(proc.pid, signal.SIGKILL))
            timer.start()
            try:
                sent = 0
                for line in iter(proc.stdout.readline, b""):
                    sent += len(line)
                    if sent > MAX_OUTPUT:
                        os.killpg(proc.pid, signal.SIGKILL)
                        out(b"\n... output truncated (over %d MB)\nFAILED\n" % (MAX_OUTPUT >> 20))
                        break
                    out(line)
                proc.wait()
            finally:
                timer.cancel()
            if proc.returncode < 0 and sent <= MAX_OUTPUT:
                out(b"\nKILLED (server run limit)\nFAILED\n")
        except (BrokenPipeError, ConnectionResetError):
            pass  # the browser went away
        finally:
            if proc and proc.poll() is None:
                try:
                    os.killpg(proc.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                proc.wait()
            run_lock.release()
        self.close_connection = True


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--host", default=os.environ.get("HOST", "127.0.0.1"))
    ap.add_argument("--port", type=int, default=int(os.environ.get("PORT", 8080)))
    a = ap.parse_args()
    if not PASSWORD and a.host not in ("127.0.0.1", "localhost", "::1"):
        sys.exit("refusing to listen on %s without DAILY_PASSWORD set "
                 "(this server runs the code it is sent)" % a.host)
    WORK.mkdir(parents=True, exist_ok=True)
    PROGRESS.touch()
    print(f"daily web on http://{a.host}:{a.port}", file=sys.stderr)
    ThreadingHTTPServer((a.host, a.port), Handler).serve_forever()


if __name__ == "__main__":
    main()
