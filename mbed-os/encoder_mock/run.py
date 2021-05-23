import sys
import Encoder

sys.argv.pop(0)
delay = float( sys.argv.pop(0))
encoder = Encoder.Encoder(19,26,delay)

for steps in sys.argv:
  steps = int(steps)
  if steps < 0:
    encoder.left(-steps)
  elif steps > 0:
    encoder.right(steps)
