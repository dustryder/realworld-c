# ![RealWorld Example App](logo.png)

> ### facil.io codebase containing real world examples (CRUD, auth, advanced patterns, etc) that adheres to the [RealWorld](https://github.com/gothinkster/realworld) spec and API.


### [Demo](https://demo.realworld.build/)&nbsp;&nbsp;&nbsp;&nbsp;[RealWorld](https://github.com/gothinkster/realworld)


This codebase was created to demonstrate a fully fledged fullstack application built with **facil.io** including CRUD operations, authentication, routing, pagination, and more.

We've gone to great lengths to adhere to the **facil.io** community styleguides & best practices.

For more information on how to this works with other frontends/backends, head over to the [RealWorld](https://github.com/gothinkster/realworld) repo.


# How it works

Typical 3-tier architecture:

HTTP layer
    |
    |
    V
Application layer
    |
    |
    V
Persistence layer

# Getting started

## Prerequisites

This project uses postgres as a database and will need to be installed. As well as this it uses dbmate to perform database migrations, although frankly you may just as well run the migration scripts directly.

Otherwise:

1. Ensure dependencies are installed correctly via:

apt update && apt install -y libpq-dev libjwt-dev

3. Run make

4. Run the executable in the build folder

Note: this relies on glibc with gnu c extensions

