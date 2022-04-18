import socket
import threading
from risovka import ris


coord = []


def refresh(data):
    if data == []:
        return 0
    data_split = data.split("|")
    n = data_split[0]
    phi = data_split[2]
    lmb = data_split[1]

    ch = False

    for LA in coord:
        if LA[0] == n:
            LA[1] = phi
            LA[2] = lmb
            ch = True

    if not ch:
        coord.append(data_split)


listener = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# IP = socket.gethostbyname(socket.gethostname())
# PORT = 12333
IP = '127.0.0.1'
PORT = 12346
listener.bind((IP, PORT))
#listener.listen(0)
print("IP:", IP, "PORT:", PORT)


# connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# IP = '127.0.0.1'
# PORT = 12346
# connection.bind((IP, PORT))

#connection, address = listener.accept()

#connection.send("Ready".encode('utf8'))
def serv_coord():
    while True:
        while True:
            data = listener.recvfrom(1024) #.decode("utf8")
            data = bytes.decode(data[0])
            refresh(data)
            if not data:
                break


threading.Thread(target=serv_coord).start()
threading.Thread(target=ris(coord)).start()