import requests

for x in range(40):
    r = requests.get('http://127.0.0.1:1800')
    print (r)
    print(r.content)


