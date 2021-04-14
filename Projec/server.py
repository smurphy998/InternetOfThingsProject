#!/usr/bin/env python
import socket
import sys
import datetime
import matplotlib.pyplot as plot
from matplotlib import animation
from Crypto.Cipher import AES


# server network configurations
SERVER_IP_ADDRESS = "192.168.1.87"
PORT = 50123

time = [0]*50
for i in range(0,50):
		time[i] = i

ax_points = [float(0)]*50
ay_points = [float(0)]*50
az_points = [float(0)]*50

print("starting UDP Server Setup")
sys.stdout.flush()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (SERVER_IP_ADDRESS, PORT) )

print("waiting for data to receive")
sys.stdout.flush()

KEY = b'3874460957140850'
iv = b'9331626268227018'

fig = plot.figure()
ax = plot.axes(xlim=(0, 50), ylim=(-2, 2))
lineX, lineY, lineZ, = ax.plot([], [], [], [], [], [], lw=2)


def init():
	lineX.set_data([], [])
	lineY.set_data([], [])
	lineZ.set_data([], [])
	return lineX, lineY, lineZ,

def updateData(i):
	decryption_suite = AES.new(KEY, AES.MODE_CBC, IV=iv)
	data, addr = sock.recvfrom(64)
	#print (''.join('{:02x}'.format(x) for x in data))
	plain_text = decryption_suite.decrypt(data)


	data = plain_text.decode('utf-8')
	print(data)
	print(i)
	sys.stdin.flush()
	if(i%2!=0): #The server somehow started producing its own values, so this needed to be implemented
		ax,ay,az,dump = data.split(",")
		print("ADXL345 X-Axis: " + ax + "\tY-Axis: " + ay + "\tZ-Axis: " + az + "\t" + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f"))
		sys.stdout.flush()

		del ax_points[0]
		del ay_points[0]
		del az_points[0]
		ax_points.append(float(ax))
		ay_points.append(float(ay))
		az_points.append(float(az))
		lineX.set_data(time,ax_points)
		lineY.set_data(time,ay_points)
		lineZ.set_data(time,az_points)
	return lineX, lineY, lineZ,

anim = animation.FuncAnimation(fig, updateData, init_func=init,
                               frames=200, interval=20, blit=True)
plot.show()
