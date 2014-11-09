ip route del default:
   cmd:
   - run

#vers VM1-4

eth1:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.156
   - netmask: 255.255.255.240
   - gateway: 172.16.2.151

tun0:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.1.1
   - netmask: 255.255.0.0

#vers VM2-6

# en statique
eth2:            
   network.managed:                                                              
   - enabled: True           
   - type: eth                                                        
   - proto: none                                                   
   - ipaddr: 192.168.2.1     # nécessaire mais bidon                        
   - netmask: 255.255.255.0               
   - enable_ipv6: True                          
   - ipv6proto: static                       
   - ipv6addr: fc00:1234:1::16      
   - ipv6netmask: 64
   - ipv6gateway: fc00:1234:1::26