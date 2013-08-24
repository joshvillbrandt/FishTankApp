#/usr/bin/python

# import django settings
import os, sys
sys.path.append('var/sites/FishTank/')
os.environ['DJANGO_SETTINGS_MODULE'] = 'settings'
import settings

# process django settings
from django.core.management import setup_environ
setup_environ(settings)

# import django models
from datetime import datetime
from FishTank.FishTankApp.models import DataLog

data_set = DataLog.objects.filter(date_logged__gt=datetime(2013, 8, 24, 11, 50))
print len(data_set)
for data in data_set:
    temp = data.temp_tank
    data.temp_tank = data.temp_ambient
    data.temp_ambient = temp
    #data.save()