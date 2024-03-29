import qubelsi.udpsendrecv

class GPIO:

    def __init__(self, addr, path):
        self.handle = qubelsi.udpsendrecv.UDPSendRecv(addr, path)

    def write_value(self, value):
        return self.handle.write_gpio(value)

    def read_value(self):
        return self.handle.read_gpio()

    def do_init(self, message_out=False):
        self.write_value(0xffff)
        return self.read_value()
