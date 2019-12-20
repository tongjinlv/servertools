#!/bin/env bash
deletePortList="端口号,以空格分隔";for port in $deletePortList;do iptables -D INPUT -p tcp --dport $port -j ACCEPT;done
