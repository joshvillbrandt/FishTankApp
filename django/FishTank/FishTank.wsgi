import os
import sys	
sys.path.append('/var/sites/')
sys.path.append('var/sites/FishTank/')
os.environ['DJANGO_SETTINGS_MODULE'] = 'FishTank.settings'
import django.core.handlers.wsgi
application = django.core.handlers.wsgi.WSGIHandler()
