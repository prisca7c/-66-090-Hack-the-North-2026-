import serial
import re
from collections import deque
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

PORT = 'COM3'
BAUD = 9600

ser = serial.Serial(PORT, BAUD, timeout=1)

WINDOW = 300 
heights = deque([512] * WINDOW, maxlen=WINDOW)

fig, ax = plt.subplots(figsize=(10, 5))
fig.patch.set_facecolor('black')
ax.set_facecolor('black')

line, = ax.plot(range(WINDOW), heights, color='#39FF14', linewidth=1.5)

ax.set_title('Hack the North 2026 \u2014 Live Profilometer Scan', color='white', fontsize=14)
ax.set_xlabel('scan position', color='white')
ax.set_ylabel('height (ADC reading)', color='white')
ax.tick_params(colors='white')
for spine in ax.spines.values():
    spine.set_color('white')
ax.set_ylim(0, 1023)

def update(frame):
    raw = ser.readline().decode(errors='ignore').strip()
    match = re.search(r'H:(\d+)', raw)
    if match:
        heights.append(int(match.group(1)))
        line.set_ydata(heights)
    return line,

ani = FuncAnimation(fig, update, interval=20, blit=True, cache_frame_data=False)
plt.tight_layout()
plt.show()
