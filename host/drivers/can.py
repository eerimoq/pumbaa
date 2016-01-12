
SPEED_250KBPS = 0
SPEED_500KBPS = 1
SPEED_1000KBPS = 2

class Frame(object):
    
    def __init__(self, frame_id, data=None, extended_id=False):
        self.id = frame_id
        if data is None:
            data = []
        self.data = data
        self.extended_id = extended_id

    def __str__(self):
        return "id = 0x{:x}, data = {}".format(self.id, self.data)


class Can(object):

    def __init__(self, device, speed):
        self.device = device
        self.speed = speed
        self.frames = []

    def start(self):
        pass

    def stop(self):
        pass

    def write(self, frame):
        print("write CAN frame '{}'".format(frame))

    def read(self):
        print("read CAN frame")
        return Frame(1)
