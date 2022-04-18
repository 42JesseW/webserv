import requests

# docker nginx
# docker run -d -p 8080:80 nginx:latest

#
resp = requests.get('http://localhost:8080/test')
print(resp.content)
print(resp.headers)
print(resp.status_code)
