FROM debian:trixie-slim AS build-env
RUN apt update && apt install -y libpq-dev libjwt-dev build-essential curl

WORKDIR /app
COPY . .
RUN make

CMD ["./build/conduit", "-log", "-b", "0.0.0.0", "-p", "3000"]