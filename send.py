import paho.mqtt.client as mqtt
import serial
import time

# Configurações do MQTT Broker
broker_address = "192.168.1.8"  # Substitua pelo endereço do seu servidor MQTT
port = 1883
topic = "rssi"  # Substitua pelo tópico desejado
client_id = "python-script"

# Configurações da porta serial
serial_port = "/dev/ttyACM0"  # Substitua pelo seu dispositivo serial (exemplo para Linux)
baud_rate = 9600

# Função para publicar dados no tópico MQTT
def publish_data(client, topic, data):
    client.publish(topic, data)
    print(f"Dados publicados no tópico {topic}: {data}")

# Cria um cliente MQTT
client = mqtt.Client(client_id)
client.connect(broker_address, port)

# Abre a porta serial
ser = serial.Serial(serial_port, baud_rate)

try:
    while True:
        # Lê dados da porta serial
        data = ser.readline().decode().strip()  # Suponha que os dados sejam uma linha de texto
        data = data[15:len(data)]
        try:
          data = int(data)
          if data:
              publish_data(client, topic, data)
        except:
          continue

except KeyboardInterrupt:
    ser.close()  # Fecha a porta serial
    client.disconnect()
    print("Desconectado do servidor MQTT")
