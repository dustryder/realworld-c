FROM amacneil/dbmate
RUN apt update && apt install -y libpq-dev build-essential

WORKDIR /app
COPY . .
RUN make

CMD ["./build/realworld", "-log"]