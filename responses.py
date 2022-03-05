import requests

response = requests.get('https://api.github.com')

# print(response.url)

# print(response.status_code)

print(response.headers)
