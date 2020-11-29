# base image
FROM node:13.10.1

WORKDIR /app

ENV PATH /app/node_modules/.bin:$PATH

COPY package.json /app/package.json
RUN npm install
RUN npm install -g @angular/cli@9.0.6

COPY . /app

CMD ng serve --host 0.0.0.0
