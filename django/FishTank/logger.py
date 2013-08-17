#/usr/bin/python

import serial
import threading
import time
from datetime import datetime

def temp_logger(ard):
  # send msg to arduino
  ard.write('T')
  
  # wait a second for reply
  time.sleep(1)
  
  # read 5 bytes from arduino (string sent)
  temp = ard.read(5)
  print temp
  
  # open file for writing data
  f = open('pythontemp_logger.txt', 'a')
  
  now_time = datetime.now()
  str_now_time = now_time.strftime("%H:%M:%S")
  f.write(str_now_time + ',' + temp + '\n')
  f.close()
  
  # start the timer again
  global t
  t = threading.Timer(10.0, logger, [ard])
  t.start()
  

# entry point
if __name__ == '__main__':
  print 'INIT: starting task'
  
  # attempt to make serial connection to arduino
  try:
    arduino = serial.Serial('/dev/tty.usbserial-A600agDn', 9600)
  except:
    print 'ERROR: whilst opening serial port'
    exit(0)
  
  # wait 2 seconds for arduino to reset
  time.sleep(3)
  
  # start the initial timer
  t = threading.Timer(10.0, temp_logger, [arduino])
  t.start()