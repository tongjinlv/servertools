#!/bin/env bash
#删除用户
sed -i "/\[mysqld\]/a\skip-grant-tables" /etc/my.cnf && service mysqld restart && mysql -e "delete from mysql.user where user='test';commit;" && sed -i "/skip-grant-tables/d" /etc/my.cnf  && service mysqld restart
