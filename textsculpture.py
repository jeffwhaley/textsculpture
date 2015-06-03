import os
import requests
import tinycss2
from tinycss2 import color3
from flask import Flask, Response, request

app = Flask(__name__)

@app.route('/', methods=['GET'])
def merry_christmas():
  return 'Merry Christmas!'