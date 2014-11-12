#!/bin/bash



ip addr add 172.16.1.1/24 dev tun0

/etc/init.d/networking restart
