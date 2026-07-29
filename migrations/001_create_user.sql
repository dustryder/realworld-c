CREATE TABLE "user" (
    "id" serial primary key,
    "username" varchar(128) unique not null,
    "email" varchar(254) unique not null,
    "password" varchar(256) not null,
    "inserted_at" timestamptz not null default (now() at time zone 'utc'),
    "updated_at" timestamptz not null default (now() at time zone 'utc')
);