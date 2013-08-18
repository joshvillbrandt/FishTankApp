#/usr/bin/python

import serial
import threading
import time
from decimal import *
#from datetime import datetime

# import django settings
import os
os.environ['DJANGO_SETTINGS_MODULE'] = 'settings'
import settings

# process django settings
from django.core.management import setup_environ
setup_environ(settings)

# import django models
from FishTank.FishTankApp.models import DataLog

def logger(ard):
  # send msg to arduino
  ard.write('T')
  
  # wait a second for reply
  time.sleep(5)
  
  # read 5 bytes from arduino (string sent)
  status = ard.read(50).split(',') # pick a high number we don't expect to get since we are non-blocking (timeout=0)
  
  if len(status) == 3:
    d = DataLog(temp_ambient = Decimal(status[1]), temp_tank = Decimal(status[2]), light_ambient = Decimal(status[0]))
    d.save()
  else:
    print 'ERROR: unrecognized serial format'
  # open file for writing data
  #f = open('pythontemp_logger.txt', 'a')
  #now_time = datetime.now()
  #str_now_time = now_time.strftime("%H:%M:%S")
  #f.write(str_now_time + ',' + temp + '\n')
  #f.close()
  
  # start the timer again
  global t
  t = threading.Timer(595.0, logger, [ard]) # every 10 minutes
  t.start()
  

# entry point
if __name__ == '__main__':
  print 'INIT: starting logger'
  
  # attempt to make serial connection to arduino
  try:
    arduino = serial.Serial('/dev/fishtank', 57600, timeout=0)
  except:
    print 'ERROR: whilst opening serial port'
    exit(0)
  
  # wait 3 seconds for arduino to reset
  time.sleep(3)
  
  # start the initial timer
  t = threading.Timer(5.0, logger, [arduino])
  t.start()