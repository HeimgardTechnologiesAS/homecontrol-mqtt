#pragma once

//#define ETHERNET // for WIZ5100
#define ETHERNET2 // for WIZ5500

#if defined(ETHERNET) || defined(ETHERNET2)
#define ETH_MAC {0x11, 0x11, 0x22, 0x33, 0x44, 0x55} // make sure that MAC is unique per device
#define ETH_IP {192, 168, 5, 40}                    // device will use this IP if it can not get ip automatically
#define ETH_DNS {192, 168, 5, 1}                    // IP of DNS server (router)
#define ETH_GATEWAY {192, 168, 5, 1}                // IP of internet gateway (router)
#define ETH_SUBNET {255, 255, 255, 0}               // subnet (if you don't know what this is leave it like this)
#endif // defined(ETHERNET) || defined(ETHERNET2)


