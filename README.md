FishTankApp
===========

A collection of tools for my desktop fish tank. This app uses an Arduino with some Sparkfun sensors to read the temperature of a fish tank. A python script running on an Ubuntu machine queries the Arduino for the data and stores it in the Django database. The Django app servers a webpage that shows the data in graphical form. The website is made with Bootstrap, jQuery, and Highcharts.

# Installation
1 make circuit
2 upload code to arduino
3 set up django app (optionally setup apache to make this permanent)
4 setup usb rules for the arduino
5 start logger (optionally use upstart to keep it going)
6 view website!

# USB rules
    sudo cp django/FishTank/99-fishtank.rules /etc/udev/rules.d/
    sudo adduser $(whoami) dialout # or group root or run logger as root
