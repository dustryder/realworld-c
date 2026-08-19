-- migrate:up
CREATE TABLE "follow" (
    "user_id" int not null,
    "user_follow_id" int not null,

    constraint follow_user_fk foreign key (user_id) references "user"(id),
    constraint follow_user_follow_fk foreign key (user_follow_id) references "user"(id),
    constraint follow_user_user_follow_pk primary key (user_id, user_follow_id)
);

-- migrate:down
DROP TABLE "follow";