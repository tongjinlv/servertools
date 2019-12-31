#!/bin/env bash
#删除用户
(/bin/sed -i "/\[mysqld\]/a\skip-grant-tables" /etc/my.cnf && /sbin/service mysql restart >>/dev/null && /usr/bin/mysql -e "flush privileges;delete from mysql.user where user='test';commit;" && /bin/sed -i "/skip-grant-tables/d" /etc/my.cnf  && /sbin/service mysql restart >>/dev/null)
