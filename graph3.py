from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

def plot_graph(dt):
	dt = dt.split(' ')
	fig = plt.figure()
	width = 10
	ax = fig.add_subplot(111, projection='3d')
	for c, z in zip(['c'], [30]):
		xs = (int(dt[0]), int(dt[1]), int(dt[2]), int(dt[3]), int(dt[4]), int(dt[5]))
		ys = (int(dt[6]), int(dt[7]), int(dt[8]), int(dt[9]), int(dt[10]), int(dt[11]))
		ax.bar(xs, ys, zs=z, zdir='y', color='b')

	ax.set_xlabel('X-axis: PnL (in millions)')
	ax.set_ylabel('Y-axis: Frequencies')
	ax.set_zlabel('Z')

	plt.show()

