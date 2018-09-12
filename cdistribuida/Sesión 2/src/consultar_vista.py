import requests
import json

resp = requests.get('http://127.0.0.1:5984/libros/_design/testlibros/_view/consultaautores?group=true&reduce=true')
if resp.status_code != 200:
    # This means something went wrong.
    raise ApiError('GET /tasks/ {}'.format(resp.status_code))

print(json.dumps(resp.json(), indent=4, sort_keys=True))

res = resp.json()

print "\nAutor - Coincidencias"

for item in res['rows']:
    print item['key'] + ' - ' + str(item['value'])
