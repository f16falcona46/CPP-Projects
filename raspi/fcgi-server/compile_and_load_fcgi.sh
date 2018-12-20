#!/bin/sh

g++ fcgi_server.cpp -lfcgi -lfcgi++ -lsqlite3 -lctemplate -lpthread -o fcgi_server
sudo service fcgi-server stop
sudo cp fcgi_server /var/www/html/cgi-bin/
sudo service fcgi-server start
