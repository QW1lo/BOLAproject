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
        lmb = data_split[1]
        phi = data_split[2]
        dal = data_split[3]
        az = data_split[4]
        h = data_split[5]
        dy = data_split[6]
        X = data_split[7]
        Z = data_split[8]
        V = data_split[9]
        PSI = data_split[10]
        TCAS = data_split[11]


        ch = False

        for LA in self.coord:
            if LA[0] == n:
                LA[1] = float(phi)
                LA[2] = float(lmb)
                LA[3] = float(dal)
                LA[4] = float(az)
                LA[5] = float(h)
                LA[6] = float(dy)
                LA[7] = float(X)
                LA[8] = float(Z)
                LA[9] = float(V)
                LA[10] = float(PSI)
                LA[11]= int(TCAS)
                ch = True

        if not ch:
            self.coord.append(data_split)
        

    def star_serv(self):
        while True:
            while True:
                data = self.listener.recvfrom(1024) #.decode("utf8")
                data = bytes.decode(data[0])
                #print("s ", data)
                if data == 'pause':
                    self.flag_stop = True
                else:
                    self.refresh(data)
                if not data:
                    break