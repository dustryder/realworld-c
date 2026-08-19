-- migrate:up
CREATE TABLE "tag" (
    "id" serial primary key,
    "name" varchar(256) not null unique
);

-- migrate:down
DROP TABLE "tag"
