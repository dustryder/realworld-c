# ![RealWorld Example App](logo.png)

> ### facil.io codebase containing real world examples (CRUD, auth, advanced patterns, etc) that adheres to the [RealWorld](https://github.com/gothinkster/realworld) spec and API.


### [Demo](https://demo.realworld.build/)&nbsp;&nbsp;&nbsp;&nbsp;[RealWorld](https://github.com/gothinkster/realworld)


This codebase was created to demonstrate a fully fledged fullstack application built with **facil.io** including CRUD operations, authentication, routing, pagination, and more.

We've gone to great lengths to adhere to the **facil.io** community styleguides & best practices.

For more information on how to this works with other frontends/backends, head over to the [RealWorld](https://github.com/gothinkster/realworld) repo.


# How it works

This uses:
* [facil.io 7.x](https://github.com/boazsegev/facil.io) for HTTP concerns
* [libpq](https://packages.debian.org/sid/libpq-dev) for postgres access
* [libjwt 2.x](https://github.com/benmcollins/libjwt) for jwt handling
* [cJSON](https://github.com/Davegamble/cjson) for JSON parsing
* [Unity](https://github.com/throwtheswitch/unity) for unit testing
* [dotenv-c](https://github.com/Isty001/dotenv-c) for env parsing


It follows typical 3-tier architecture:

Presentation layer -> Application layer -> Persistence layer


# Getting started

Note: only tested on debian-based linux

## Local

1. Ensure dependencies are installed correctly via:

```
apt update && apt install -y libpq-dev libjwt-dev postgresql
```

If wishing to use dbmate to interact with the database then this will need to be installed via

```console
sudo curl -fsSL -o /usr/local/bin/dbmate https://github.com/amacneil/dbmate/releases/latest/download/dbmate-linux-amd64
sudo chmod +x /usr/local/bin/dbmate
```

2. Create an env file and populate it with the correct details using .env.example as a template

3. Apply the migration scripts. If this is done with dbmate, just run
```console
dbmate up
```

Otherwise, just apply the scripts in ./db/migrations

4. Run
```console
make && ./build/conduit
```

## Docker

For this, you'll need to install docker on your system. After that it's as simple as

```console
docker compose up api // to spin up the entire stack needed for the api
docker compose up test-api // to spin up the bruno test collection
docker compose up test-unit // to execute the unit tests
```