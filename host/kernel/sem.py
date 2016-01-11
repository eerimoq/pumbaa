import threading
import python.pytime as time

class Sem():
    
    def __init__(self, count=1):
        self.semaphore = threading.Semaphore(count)

    def get(self, timeout=None):
        start_time = time.time()
        while True:
            if self.semaphore.acquire(False):
                return
            if timeout is not None:
                if time.time() - start_time > timeout:
                    raise
            time.sleep(0.0)
            

    def put(self, value=1):
        assert value == 1
        self.semaphore.release()
