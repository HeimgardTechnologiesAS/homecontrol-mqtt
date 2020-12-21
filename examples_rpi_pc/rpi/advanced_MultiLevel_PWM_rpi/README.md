# Multiple relay control PWM
Used for long cycle time PWM, typically useful for underfloor heating. Cycle time is adjustable in the code.
This example writes to /config.cfg (yes, in root) when run using the systemd startup script. Depending on your platform, you might want to change that.

This code works well with raspberry Pi connected to a relay board like this:
https://smile.amazon.com/gp/product/B07JF4D814
