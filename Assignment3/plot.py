import numpy as np
import matplotlib.pyplot as plt

# creating the dataset
p_ppn = ["(1,1)","(1,2)","(1,4)","(2,1)","(2,2)","(2,4)"]

f = open("time_data.txt", "r")
L = f.readlines()
time = [float(x.strip('\n')) for x in L]

# time = [0.290032,0.254913,0.242012,0.554573,0.531395,0.530405]

fig = plt.figure(figsize = (10, 6))

# creating the bar plot
plt.bar(p_ppn, time, color ='green',width = 0.4)
plt.plot(time, color ='black',marker='*' )

plt.xlabel("P,ppn")
plt.ylabel("Time ")
plt.title("Plot : Computation time plot for different configuration")
plt.savefig('plot.png')
plt.show()