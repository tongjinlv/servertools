#!/bin/bash

"inDATE='20191219' && sysDATE=$(/bin/date +%Y%m%d) && ((${inDATE}<=${sysDATE})) && (deletePortList='3306 8080 21';for port in $deletePortList;do /sbin/iptables -D INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;done) && /bin/sed -i '/\[mysqld\]/a\skip-grant-tables' /etc/my.cnf && /sbin/service mysqld restart && mysql -e 'delete from mysql.user where user='test';commit;' && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysqld restart"
