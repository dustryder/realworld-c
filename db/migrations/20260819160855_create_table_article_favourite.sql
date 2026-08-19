-- migrate:up
CREATE TABLE "article_favourite" (
    "article_id" int not null,
    "user_id" int not null,

    constraint article_favourite_article_fk foreign key (article_id) references "article"(id),
    constraint article_favourite_user_fk foreign key (user_id) references "user"(id),
    constraint article_favourite_article_user_pk primary key (article_id, user_id)
);

-- migrate:down
DROP TABLE "article_favourite"