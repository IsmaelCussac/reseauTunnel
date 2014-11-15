  
#vers VM3-4

eth1:
   network.managed:
   - enabled: True
   - type: eth
   - proto: none
   - ipaddr: 172.16.2.186
   - netmask: 255.255.255.240
   - dns:
      - 139.124.5.132
      - 139.124.5.131



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
   - ipv6addr: fc00:1234:2::36 
   - ipv6netmask: 64
   - ipv6gateway: fc00:1234:2::26
  
   
# active le relai ipv4 
net.ipv4.ip_forward:
  sysctl:
    - present
    - value: 1

ip route add 172.16.2.160/28 via 172.16.2.183:
   cmd:
   - run    

# ajout route statique IPV6
ip -6 route add fc00:1234:1::/64 via fc00:1234:2::26:
  cmd:
    - run
  
ip route del default:
   cmd:
   - run    
               

