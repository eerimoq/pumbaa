import threading

def spawn(func, args=(), prio=0):
    thread = threading.Thread(target=func, args=args)
    thread.start()
    return thread

def suspend():
    pass
