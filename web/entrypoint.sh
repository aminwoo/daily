#!/bin/sh
# Hosts like Render mount the /data disk owned by root: hand it to the
# unprivileged user, then run the server as that user.
set -e
if [ "$(id -u)" = 0 ]; then
  chown daily:daily /data
  exec setpriv --reuid=daily --regid=daily --init-groups "$@"
fi
exec "$@"
