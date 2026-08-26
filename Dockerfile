FROM debian:trixie-slim AS build-env
RUN apt update && apt install -y libpq-dev libjwt-dev build-essential curl

WORKDIR /app
COPY . .
RUN make
