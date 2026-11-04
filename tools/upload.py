import sys
import time
import serial

PORT = "/dev/ttyUSB1"
BAUD = 115200
MAX_WORDS = 256

if len(sys.argv) < 2:
    sys.exit("usage: upload.py prog.hex [port]")

port = sys.argv[2] if len(sys.argv) > 2 else PORT

words = []
with open(sys.argv[1]) as f:
    for line in f:
        line = line.strip()
        if line:
            words.append(int(line, 16))

if not 0 < len(words) <= MAX_WORDS:
    sys.exit(f"program has {len(words)} words, allowed 1..{MAX_WORDS}")

data = bytearray(b"MNPK")
data.append(len(words) >> 8)
data.append(len(words) & 0xFF)
chk = 0
for w in words:
    hi = w >> 8
    lo = w & 0xFF
    data.append(hi)
    data.append(lo)
    chk ^= hi ^ lo
data.append(chk)

with serial.Serial(port, BAUD, timeout=0.5) as s:
    time.sleep(0.1)
    s.reset_input_buffer()
    for b in data:
        s.write(bytes([b]))
        s.flush()
        time.sleep(0.001)

print(f"sent {len(words)} words, {len(data)} bytes, checksum 0x{chk:02X}")
