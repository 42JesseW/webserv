import requests

for x in range(200):
    r1 = requests.get('http://127.0.0.1:8082')
    # r2 = requests.get('http://127.0.0.1:8083')
    # r3 = requests.get('http://127.0.0.1:8084')
    print (r1)
    print(r1.content)
    # print (r2)
    # print(r2.content)
    # print (r3)
    # print(r3.content)

