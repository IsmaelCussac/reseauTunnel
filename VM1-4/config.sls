
#vers VM2-4

eth1:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.131
   - netmask: 255.255.255.240
   - dns:
      - 139.124.5.132
      - 139.124.5.131
   

    
#Vers VM1-6 

eth2:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.151
   - netmask: 255.255.255.240
   - gateway: 172.16.2.156
   
   
   
ip route add 172.16.2.160/28 via 172.16.2.132:
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
