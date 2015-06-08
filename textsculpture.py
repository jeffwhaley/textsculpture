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

if __name__ == '__main__':
	app.run
	