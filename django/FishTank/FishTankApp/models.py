from django.db import models

DATE_FORMAT = "%Y-%m-%d" 
TIME_FORMAT = "%H:%M:%S"

# Create your models here.
class DataLog(models.Model):
    date_logged = models.DateTimeField(auto_now_add=True)
    temp_ambient = models.DecimalField(max_digits=5, decimal_places=2)
    temp_tank = models.DecimalField(max_digits=5, decimal_places=2)
    light_ambient = models.DecimalField(max_digits=5, decimal_places=2) # whoops, this should be light_tank
    
    def __unicode__(self):
        return self.date_logged.strftime("%s %s" % (DATE_FORMAT, TIME_FORMAT))