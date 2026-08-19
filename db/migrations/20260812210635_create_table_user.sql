-- migrate:up
CREATE TABLE "user" (
    "id" serial primary key,
    "username" varchar(128) unique not null,
    "email" varchar(254) unique not null,
    "password" varchar(256) not null,
    "bio" varchar,
    "image" varchar
);

-- migrate:down
DROP TABLE "user";