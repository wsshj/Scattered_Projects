#coding=utf-8
import requests

url = 'http://127.0.0.1:8000/sql/'
data = {'infoType': 1, 'Name': '976坝陵线旱西关街A1箱变A101变压器'}

res = requests.post(url, data=data)
print(res.text)