-- migrate:up
CREATE TABLE "article" (
    "id" serial primary key,
    "slug" varchar(256) not null unique,
    "title" varchar(256) not null,
    "body" varchar not null,
    "description" varchar(256) not null,
    "created_at" timestamp not null default (now() at time zone 'utc'),
    "updated_at" timestamp not null default (now() at time zone 'utc'),
    "created_by" int not null,

    constraint "article_created_by" foreign key (created_by) references "user"(id)
);

-- migrate:down
DROP TABLE "article"
