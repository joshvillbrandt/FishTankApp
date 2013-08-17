#/usr/bin/python

import serial
import threading
import time
from datetime import datetime

def logger(ard):
  # send msg to arduino
  ard.write('T')
  
  # wait a second for reply
  time.sleep(5)
  
  # read 5 bytes from arduino (string sent)
  status = ard.read(50) # pick a high number we don't expect to get since we are non-blocking (timeout=0)
  print status
  
  # open file for writing data
  #f = open('pythontemp_logger.txt', 'a')
  #now_time = datetime.now()
  #str_now_time = now_time.strftime("%H:%M:%S")
  #f.write(str_now_time + ',' + temp + '\n')
  #f.close()
  
  # start the timer again
  global t
  t = threading.Timer(10.0, logger, [ard])
  t.start()
  

# entry point
if __name__ == '__main__':
  print 'INIT: starting task'
  
  # attempt to make serial connection to arduino
  try:
    arduino = serial.Serial('/dev/fishtank', 57600, timeout=0)
  except:
    print 'ERROR: whilst opening serial port'
    exit(0)
  
  # wait 3 seconds for arduino to reset
  time.sleep(3)
  
  # start the initial timer
  t = threading.Timer(10.0, logger, [arduino])
  t.start()