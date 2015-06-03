import os
from flask import Flask, Response, request
import requests
import tinycss2
from tinycss2 import color3

app = Flask(__name__)
@app.route('/',methods = ['GET'])
def merry_christmas():
	return 'Merry Christmas!'
	
if __name__ == '__main__':
	app.run()

