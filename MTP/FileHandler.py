import time
from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler

class FileHandler(PatternMatchingEventHandler):
    patterns = ["*.csv"]

    def process(self, event):
        print(event.src_path, event.event_type)  # print now only for degug

    def on_modified(self, event):
        self.process(event)

    def on_created(self, event):
        self.process(event)