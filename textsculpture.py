import os
import requests
# import venv.lib.python2\.7.site-packages.requests.pip._vendor.requests
import tinycss2
# from tinycss2 import color3
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
		return Response("Sorry, I don't recognize this color",  mimetype='text/plain')
		
	if len(rgba) == 4:
		red = int(round(255*rgba[0]))
		green = int(round(255*rgba[1]))
		blue = int(round(255*rgba[2]))
		rgba_string = '[{0:03d},{1:03d},{2:03d}]'.format(red, green, blue)

if __name__ == '__main__':
	app.run
	