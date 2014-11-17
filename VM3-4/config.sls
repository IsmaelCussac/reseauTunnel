
#vers VM2-4

eth1:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.163
   - netmask: 255.255.255.240

 
#vers VM3-6
     
eth2:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.183
   - netmask: 255.255.255.240
   - gateway: 172.16.2.186


ip route add 172.16.2.128/28 via 172.16.2.162:
   cmd:
   - run   
   
ip route add 172.16.2.144/28 via 172.16.2.186:
   cmd:
   - run

   
# active le relai ipv4 
net.ipv4.ip_forward:
  sysctl:
    - present
    - value: 1
   
    
ip route del default:
   cmd:
   - run
   
