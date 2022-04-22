import socket

class serv():
    def __init__(self) -> None:
        self.listener = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        IP = '127.0.0.1'
        PORT = 12346
        self.listener.bind((IP, PORT))
        print("IP:", IP, "PORT:", PORT)
        self.coord = []
        self.flag_stop = False

    def refresh(self, data):
        if data == []:
            return 0
        data_split = data.split("|")
        n = data_split[0]
        phi = data_split[2]
        lmb = data_split[1]

        ch = False

        for LA in self.coord:
            if LA[0] == n:
                LA[1] = phi
                LA[2] = lmb
                ch = True

        if not ch:
            self.coord.append(data_split)
        

    def star_serv(self):
        while True:
            while True:
                data = self.listener.recvfrom(1024) #.decode("utf8")
                data = bytes.decode(data[0])
                print(data)
                if data == 'pause':
                    self.flag_stop = True
                else:
                    self.refresh(data)
                if not data:
                    break