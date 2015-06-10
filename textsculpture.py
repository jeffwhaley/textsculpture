import os
import requests
import tinycss2
from tinycss2 import color3
from flask import Flask, Response, request

app = Flask(__name__)

@app.route('/', methods=['GET'])
def merry_christmas():
  return 'Merry Christmas!'
  
@app.route('/sms', methods=['POST'])
def sms():
	body = request.values.get('Body', None)
	sms = body.lower()
	print sms
	rgba = tinycss2.color3.parse_color(sms)
	
	if rgba is None:
		data_string = sms
	elif len(rgba) == 4:
		red = int(round(255*rgba[0]))
		green = int(round(255*rgba[1]))
		blue = int(round(255*rgba[2]))
		data_string = '[{0:03d},{1:03d},{2:03d}]'.format(red, green, blue)
	else:
		data_string = 'error'
		
	payload = {'access_token':os.environ['SPARK_ACCESS_TOKEN'], 'command':data_string}
	r = requests.post("https://api.spark.io/v1/devices/{0}/color".format(os.environ['SPARK_CORE_ID']), data = payload)
	return Response(mimetype='text/plain')

if __name__ == '__main__':
	app.run
	