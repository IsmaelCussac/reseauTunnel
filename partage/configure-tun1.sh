#!/bin/bash

ip addr add 172.16.1.1/255.255.255.0 dev tun0

ip link set tun0 up

