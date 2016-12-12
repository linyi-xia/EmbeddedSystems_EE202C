import urllib2
import urllib

ip = '169.232.190.239'
port = '8080'
path = '/database?id='
#constant car ID
CARID = '1001'
url = 'http://'+ip+':'+port+path+CARID
print url
response = urllib2.urlopen(url)
html = response.read()
print html
#html contains json
