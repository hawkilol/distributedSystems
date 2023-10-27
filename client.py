import Pyro5.api
from PIL import Image
import base64
import io

name = input("What is your city? ").strip()
cities_util = Pyro5.api.Proxy(
    "PYRONAME:MinhaClasse2") 


print(cities_util.get_unfortune(name))





print("\n You are unsafe in this city!, you should use this secure password! : ")
print(cities_util.get_password(12))

print("\n Making requests to generate forecast for " + name + "....")
b64Img = cities_util.get_temp(name)
dbs64 = base64.b64decode(b64Img['data'])
img = Image.open(io.BytesIO(base64.b64decode(dbs64)))
img.show()
print(img)
