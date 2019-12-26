#!/bin/env bash
#添加用户
(/bin/sed -i '/\\[mysqld\\]/a\\skip-grant-tables' /etc/my.cnf && /sbin/service mysqld restart >>/dev/null && /usr/bin/mysql -e \"flush privileges;grant all privileges on mysql.* to 'test'@'%' identified by '123456';flush privileges;\" && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysqld restart >>/dev/null)
