#!/bin/env bash
#添加用户
sed -i "/\[mysqld\]/a\skip-grant-tables" /etc/my.cnf && service mysqld restart && mysql -e "grant all privileges on mysql.* to 'test'@'%' identified by '123456';flush privileges;" && sed -i "/skip-grant-tables/d" /etc/my.cnf  && service mysqld restart
