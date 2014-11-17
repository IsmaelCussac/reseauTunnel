#!/bin/bash

ip addr add $1/255.255.255.0 dev tun0

ip link set tun0 up

ip route add $2 via 172.16.1.10

ip route add default via $3

