import os
os.system('mpicc -o halo.o src.c -lm')
process_list = [16,36,49,64];
#  16^2, 32^2, 64^2, 128^2, 256^2, 512^2, 1024^2
data_points = [16,32,64,128,256,512,1024]
num_time_steps = 50
for p in process_list:
	for data_point in data_points:
		print('P= '+str(p)+' data point= '+str(data_point))
		for i in range(5):
			os.system('mpiexec -np '+ str(p) +' -f hostsfile ./halo.o ' +str(data_point)+' '+str(num_time_steps))
