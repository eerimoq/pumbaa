#
# @section License
#
# The MIT License (MIT)
# 
# Copyright (c) 2016, Erik Moqvist
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This file is part of the Pumbaa project.
#


import os
import _thread

import board
from kernel import Timer
from sync import Event
from drivers import Dac

#  Player states.
STATE_PLAYING = 0
STATE_PAUSED  = 1
STATE_STOPPED = 2

# Music player events.
EVENT_PLAY    = 0x01
EVENT_PAUSE   = 0x02
EVENT_STOP    = 0x04
EVENT_NEXT    = 0x08
EVENT_PREV    = 0x10
EVENT_TIMEOUT = 0x20
EVENT_ALL     = 0xff


class MusicPlayer(object):

    def __init__(self):
        self.state = STATE_STOPPED
        self.dac = Dac([Board.PIN_DAC0, Board.PIN_DAC1], 10000)
        self.event = Event()
        self.playlist = os.listdir()
        self.index = 0
        self.fsong = None
        self.timer = None
        self.identity = None

    def main(self):
        # Start the periodic fill timer.
        self.timer = Timer(0.1,
                           self.event,
                           EVENT_TIMEOUT,
                           flags=Timer.PERIODIC)
        self.timer.start()

        print('Pumbaa Music Player!')

        print('Playlist:')
        for number, song in enumerate(self.playlist, 1):
            print("{}: {}".format(number, song))

        # Start the main loop of the music player.
        while True:
            mask = self.event.read(EVENT_ALL)

            if mask & EVENT_STOP:
                self.handle_event_stop()

            if mask & EVENT_PAUSE:
                self.handle_event_pause()

            if mask & EVENT_NEXT:
                self.handle_event_next()

            if mask & EVENT_PREV:
                self.handle_event_prev()

            if mask & EVENT_PLAY:
                self.handle_event_play()

            # Play if the state in playing, eyy!
            if self.state == STATE_PLAYING:
                self.play_chunk()

    def start(self):
        if self.identity is None:
            self.identity = _thread.start_new_thread(self.main, ())

    def play(self):
        self.event.write(EVENT_PLAY)

    def pause(self):
        self.event.write(EVENT_PAUSE)

    def stop(self):
        self.event.write(EVENT_STOP)

    def next(self):
        self.event.write(EVENT_NEXT)

    def prev(self):
        self.event.write(EVENT_PREV)

    def handle_event_play(self):
        path = self.playlist[self.index]

        if self.state == STATE_STOPPED:
            self.fsong = open(path, "rb")

        self.state = STATE_PLAYING
        print("Playing |", path)

    def handle_event_pause(self):
        if self.state == STATE_PLAYING:
            print("Paused  |", self.playlist[self.index])
            self.state = STATE_PAUSED

    def handle_event_stop(self):
        if self.state in [STATE_PLAYING, STATE_PAUSED]:
            self.fsong.close()
            self.state = STATE_STOPPED

    def handle_event_next(self):
        if self.state in [STATE_PLAYING, STATE_PAUSED]:
            self.fsong.close()
            self.index += 1
            self.index %= len(self.playlist)
            self.fsong = open(self.playlist[self.index], "rb")

    def handle_event_prev(self):
        if self.state in [STATE_PLAYING, STATE_PAUSED]:
            self.fsong.close()
            self.index -= 1
            self.index %= len(self.playlist)
            self.fsong = open(self.playlist[self.index], "rb")

    def play_chunk(self):
        samples = self.fsong.read(1024)

        if len(samples) > 0:
            self.dac.async_convert(samples)
        else:
            self.handle_event_next()
            print("Playing |", self.playlist[self.index])
