FishTankApp
===========

A collection of tools for my desktop fish tank. This app uses an Arduino with some Sparkfun sensors to read the temperature of a fish tank. A python script running on an Ubuntu machine queries the Arduino for the data and stores it in the Django database. The Django app servers a webpage that shows the data in graphical form. The website is made with Bootstrap, jQuery, and Highcharts.

# Installation
1. make circuit and hook up sensors
2. upload code to arduino and connect it to the computer
3. set up django app (optionally setup apache to make this permanent)
4. setup usb rules for the arduino (and restart)
5. start logger (optionally use upstart to keep it going)
6. run the Django server
7. view the website!

# USB rules
Set up a device shortcut like so:

    sudo cp django/FishTank/99-fishtank.rules /etc/udev/rules.d/
    #sudo adduser $(whoami) dialout # or group root or run logger as root

Unfortunately, the symbolic link we created didn't maintain the dailup group, so only root has permission to access this device. I'm working around this by just having root run the logger. :x
