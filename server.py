import socket
from _thread import *

HOST = '192.168.137.165'
PORT = 8000
server = socket.socket()
server.bind((HOST, PORT))
server.listen(5)
distance = 0

responce = ""
def calculateDistance(dist):
    if dist <= 10:
        result = "RED"
    elif dist > 10 and dist <= 20:
        result = "YELLOW"
    elif dist > 20:
        result = "GREEN"
    return result

def funcionalidad(connection):
    try:
        while True:
            global distance
            global responce
            data = connection.recv(1024).decode('utf-8')  
            if not data:
                break
            print(f"CLIENT SENDS: {data}")
            if data.startswith('distance='):
                distance = float(data[9:])
                responce = calculateDistance(distance)
                print(responce)
                print("distance:", distance)
            elif data.startswith('GET'):
                connection.sendall(str(responce).encode())
            else:
                print("Incorrect data")
                break
    except Exception as e:
        print(f"Error in funcionalidad: {e}")
    finally:
        connection.close()

while True:
    client, addr = server.accept()
    print(f"Conexión entrante desde {addr[0]}:{addr[1]}")
    start_new_thread(funcionalidad, (client,))
