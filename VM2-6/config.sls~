sysctl -w net.ipv6.conf.all.forwarding=1:
  cmd:
    - run
    
ip route del default:
   cmd:
   - run

#vers VM1-6

# en statique
eth1:            
   network.managed:                                                              
   - enabled: True           
   - type: eth                                                        
   - proto: none                                                   
   - ipaddr: 192.168.2.1     # nécessaire mais bidon                        
   - netmask: 255.255.255.0               
   - enable_ipv6: True                          
   - ipv6proto: static                       
   - ipv6addr: fc00:1234:1::26      
   - ipv6netmask: 64
   - ipv6gateway: fc00:1234:1::16
  
#vers VM3-6
  
eth2:            
   network.managed:                                                              
   - enabled: True           
   - type: eth                                                        
   - proto: none                                                   
   - ipaddr: 192.168.2.1     # nécessaire mais bidon                        
   - netmask: 255.255.255.0               
   - enable_ipv6: True                          
   - ipv6proto: static                       
   - ipv6addr: fc00:1234:2::26      
   - ipv6netmask: 64
   - ipv6gateway: fc00:1234:2::36         

