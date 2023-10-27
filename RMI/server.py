import Pyro5.api
import random
import requests
from bs4 import BeautifulSoup
import matplotlib.pyplot as plt
import pandas as pd
import json
import climage
from PIL import Image
import base64
import io

@Pyro5.api.expose
class MinhaClasse2(object):

  def get_unfortune(self, name):
    randomInt = str(random.randint(0, 10))
    randomInt = str(random.randint(0, ord(name[0])))
    return "Hello YOU from, {0}. Here is your unfortune message:\n \ Tomorrow's unlucky number is {1}.".format(
        name, randomInt)

  def get_password(self, length=16):
    """Gera uma senha aleatoria a partir de uma tamanho.
    """
    
    lower_letters = 'abcdefghijklmnopqrstuvwxyz'
    upper_letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    numbers = '0123456789'
    special_chars = '!@#$%^&*()_+{}:"<>?~'

    characters = lower_letters + upper_letters + numbers + special_chars

    password = ''
    for i in range(length):
      password += random.choice(characters)
    return password

  def get_temp(self, name):
    """Retorna o grafico com dados de previsão de tempo,
    a partir do nome de uma cidade, ele busca as coodenadas de uma
    cidade em uma api, usa a latitude e logitude em outra api e plota esses
    dados em graficos e retorna ao cliente uma imagem em base64 do plot.
    """
    url = "https://api.teleport.org/api/cities/?search=" + name
    response = requests.get(url)
    data = json.loads(response.text)
  
    city_href = data['_embedded']['city:search-results'][0]['_links'][
        'city:item']['href']
  
    response = requests.get(city_href)
    data_latlon = json.loads(response.text)
  
    lat = data_latlon['location']['latlon']['latitude']
    lon = data_latlon['location']['latlon']['longitude']

    url = "https://ws1.metcheck.com/ENGINE/v9_0/json.asp?lat={0}&lon={1}&Fc=No".format(
        lat, lon)
    response = requests.get(url)
    data_temp = json.loads(response.text)
    print(data_temp)
  
    df = pd.DataFrame(data_temp['metcheckData']['forecastLocation']['forecast'])
  
    temperature = df['temperature']
    rain = df['rain']
    uvIndex = df['uvIndex']
    chanceofrain = df['chanceofrain']
    totalcloud = df['totalcloud']
    time = df['utcTime']
  
    time = pd.to_datetime(time)
  
    fig, axes = plt.subplots(3, 1, figsize=(10, 6))
  
    axes[0].plot(time, temperature)
    axes[0].set_ylabel('Temperatura (°C)')
    axes[0].set_title('Previsão do tempo')
  
    axes[1].plot(time, rain)
    axes[1].set_ylabel('Chuva (mm)')
  
    axes[2].plot(time, uvIndex)
    axes[2].set_ylabel('Indice raios UV')
  
    for ax in axes:
        ax.tick_params(labelsize=7)
        ax.set_xlabel('Dia')
  
    for ax in axes:
        ax.grid(True)

    buffer = io.BytesIO()
    fig.savefig(buffer, format="png")

    b64 = base64.b64encode(buffer.getvalue())

    return b64

daemon = Pyro5.server.Daemon()  # make a Pyro daemon
ns = Pyro5.api.locate_ns()  # find the name server
uri = daemon.register(
MinhaClasse2)  # register the greeting maker as a Pyro object
ns.register("MinhaClasse2",
            uri)  
print("Ready.")

daemon.requestLoop()  
