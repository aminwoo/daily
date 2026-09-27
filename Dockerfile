# daily web: the exercise set as a website. See "Web version" in README.md.
# trixie: GCC 14. Older libasan (bookworm's GCC 12) randomly dies with an endless
# "AddressSanitizer:DEADLYSIGNAL" loop on kernels with high-entropy ASLR.
FROM debian:trixie-slim
RUN apt-get update \
 && apt-get install -y --no-install-recommends g++ python3 bash gawk ca-certificates \
 && rm -rf /var/lib/apt/lists/*
RUN useradd --create-home --uid 1000 daily && mkdir /data && chown daily /data
WORKDIR /app
COPY --chown=daily . .
RUN chown daily /app
# your solutions and progress live on the /data volume, not in the image
ENV DAILY_WORK=/data/work DAILY_PROGRESS=/data/progress HOST=0.0.0.0 PORT=8080
VOLUME /data
EXPOSE 8080
# starts as root only to chown the /data disk, then drops to `daily`
ENTRYPOINT ["web/entrypoint.sh"]
CMD ["python3", "web/server.py"]
