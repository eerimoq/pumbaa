
OUTPUT = 0
INPUT = 1

class Pin(object):

    def __init__(self, device, mode):
        self.device = device
        self.mode = mode
        self.state = "off"

    def __str__(self):
        return "Pin {} in state {}.".format(self.device, self.state)

    def toggle(self):
        if self.state == "off":
            self.state = "on"
        else:
            self.state = "off"
        print(self)
