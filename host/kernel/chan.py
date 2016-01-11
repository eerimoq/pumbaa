import python.pytime as time

class List():
    
    def __init__(self, channels):
        self.channels = channels

    def poll(self):
        while True:
            for channel in self.channels:
                if not channel.empty():
                    return channel
                time.sleep(0.0)
