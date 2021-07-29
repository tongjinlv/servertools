#!/bin/env bash
allowPortList="3306 8080 21";for port in $allowPortList;do iptables -I INPUT -p tcp --dport $port -j ACCEPT;done
