from gpiozero import LED
from time import sleep

class Encoder:
  def __init__(self, clkPin, dtPin, delay):
    self.clk = LED(clkPin)
    self.dt  = LED(dtPin)
    self.delay = delay
    self.states = [0,1,3,2]
    self.stateIdx = 0

  def publish(self):
    if self.states[self.stateIdx] & ( ( 1 << 0 )):
        self.dt.on();
    else:
        self.dt.off();
    if self.states[self.stateIdx] & ( ( 1 << 1 )):
        self.clk.off();
    else:
        self.clk.on();
    sleep(self.delay)

  def left(self,steps):
    for _x in range(0,steps):
      self.stateIdx = self.stateIdx - 1
      if self.stateIdx == -1:
         self.stateIdx = 3
      self.publish()

  def right(self,steps):
    for _x in range(0,steps):
      self.stateIdx = self.stateIdx + 1
      if self.stateIdx == 4:
         self.stateIdx = 0
      self.publish()
