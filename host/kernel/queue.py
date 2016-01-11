import Queue as PQueue

class Queue():
    
    def __init__(self):
        self.fifo = []
        self.queue = PQueue.Queue()

    def read(self):
        return self.queue.get()

    def write(self, obj):
        self.queue.put(obj)

    def empty(self):
        return self.queue.empty()
