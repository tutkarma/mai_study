#!/bin/bash

docker-compose build
docker-compose up --remove-orphans --exit-code-from=app app
