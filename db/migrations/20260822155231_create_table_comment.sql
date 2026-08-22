-- migrate:up
CREATE TABLE "comment" (
    "id" serial primary key,
    "article_id" int not null,
    "created_by" int not null,
    "body" varchar not null,
    "created_at" timestamp not null default (now() at time zone 'utc'),
    "updated_at" timestamp not null default (now() at time zone 'utc'),

    constraint fk_comment_article_id foreign key (article_id) references "article"(id) on delete cascade,
    constraint fk_comment_user_id foreign key (created_by) references "user"(id) on delete cascade
);

-- migrate:down
DROP TABLE "comment"