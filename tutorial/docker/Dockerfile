FROM nginx:latest

RUN apt-get update \
    && apt-get install -y vim

COPY default.conf /etc/nginx/conf.d/default.conf

RUN mkdir /usr/share/nginx/html/lol \
    && cp /usr/share/nginx/html/index.html /usr/share/nginx/html/lol/index.html \
    && echo "<h1>LOL</h1>" >> /usr/share/nginx/html/lol/index.html
