from django.template import Context, RequestContext, loader
from django.http import HttpResponse
from FishTankApp.models import *
from utils import JSONResponse
#from django.utils import simplejson
from datetime import datetime, timedelta

def index(request):
    t = loader.get_template('index.html')
    c = RequestContext(request, {
#        'nav': 'index',
#        'username': __user(request),
    })
    return HttpResponse(t.render(c))

def json(request, action = None):
    response = {
        "action": action,
        "result": 'failure',
        "message": '',
    }
    
    if action == 'getData':
        response['result'] = 'success'
        response['data'] = []
        if int(request.GET['days']) < 1:
            datalog_set = DataLog.objects.all().order_by('date_logged')
        else:
            t = datetime.now() - timedelta(days=int(request.GET['days']))
            datalog_set = DataLog.objects.filter(date_logged__gte=t).order_by('date_logged')
        for datalog in datalog_set:
            d = {}
            d['date_logged'] = datalog.date_logged
            d['temp_ambient'] = datalog.temp_ambient
            d['temp_tank'] = datalog.temp_tank
            d['light_ambient'] = datalog.light_ambient
            response['data'].append(d)
    
    elif action == 'getSettings':
        pass
    
    elif action == 'setSettings':
        pass
    
    callback = None
    if 'callback' in request.GET:
        callback = request.GET['callback']
    
    return JSONResponse(response, jsonp=callback)