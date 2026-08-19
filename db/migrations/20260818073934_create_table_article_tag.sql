-- migrate:up
CREATE TABLE "article_tag" (
    "article_id" int not null,
    "tag_id" int not null,

    constraint article_tag_article_fk foreign key (article_id) references "article"(id),
    constraint article_tag_tag_fk foreign key (tag_id) references "tag"(id),
    constraint article_tag_article_tag_pk primary key (article_id, tag_id)
);

-- migrate:down
DROP TABLE "article_tag"