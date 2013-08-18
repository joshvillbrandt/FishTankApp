from django.conf.urls.defaults import *

urlpatterns = patterns('FishTankApp.views',
    url(r'^$', 'index'),
    url(r'^json/(.*)/$', 'json'),
)