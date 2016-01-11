import threading
import python.pytime as time

def spawn(func, args=(), prio=0):
    thread = threading.Thread(target=func, args=args)
    thread.start()
    return thread

def suspend():
    pass

def sleep(microseconds):
    time.sleep(microseconds / 1000)
