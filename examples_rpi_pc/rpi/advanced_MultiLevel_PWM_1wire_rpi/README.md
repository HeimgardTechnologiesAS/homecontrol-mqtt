# Multiple relay control PWM + 1Wire

Relays used for long cycle time PWM, typically useful for underfloor heating. Cycle time is adjustable in the code.

This example writes to /home/pi/hc-mqtt/config.cfg when run using the systemd startup script. Depending on your platform, you might want to change that.

To enable 1 wire, run
"sudo raspi-config"
Then interface->1-wire and enable it.

This code works well with raspberry Pi connected to a relay board like this:
https://smile.amazon.com/gp/product/B07JF4D814
