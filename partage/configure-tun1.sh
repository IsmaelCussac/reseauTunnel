#!/bin/bash

ip addr add $1/255.255.255.0 dev tun0

ip link set tun0 up

ip route add 172.16.2.144/28 via 172.16.1.10

ip route add default via 172.16.1.1
