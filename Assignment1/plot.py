# Import libraries 
import numpy as np 
import statistics
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt 
import math

# print("hello")

# # 16 process with data points
# a16 = [1.0,1.5,1.2,1.1,0.8]
# a32 = [1.5,1.7,2.0,1.6,1.8]
# a49 = [1.4,1.6,1.2,1.0,1.7]
# a64 = [2,2.05,1.9,1.8,2.5]
# a = [a16,a32,a49,a64]


# m16 = statistics.median(a16)
# m32 = statistics.median(a32)
# m49 = statistics.median(a49)
# m64 = statistics.median(a64)
# # m16,m32,m49,m64

file1 = open("data_back_up.txt","r") 
# 16 32 64 128 256 512 1024
p16= {
		'16p':[],
		'32p':[],
		'64p':[],
		'128p':[],
		'256p':[],
		'512p':[],
		'1024p':[],
		'16v':[],
		'32v':[],
		'64v':[],
		'128v':[],
		'256v':[],
		'512v':[],
		'1024v':[],
		'16s':[],
		'32s':[],
		'64s':[],
		'128s':[],
		'256s':[],
		'512s':[],
		'1024s':[],
}
p36={
		'16p':[],
		'32p':[],
		'64p':[],
		'128p':[],
		'256p':[],
		'512p':[],
		'1024p':[],
		'16v':[],
		'32v':[],
		'64v':[],
		'128v':[],
		'256v':[],
		'512v':[],
		'1024v':[],
		'16s':[],
		'32s':[],
		'64s':[],
		'128s':[],
		'256s':[],
		'512s':[],
		'1024s':[],
}
p49={
		'16p':[],
		'32p':[],
		'64p':[],
		'128p':[],
		'256p':[],
		'512p':[],
		'1024p':[],
		'16v':[],
		'32v':[],
		'64v':[],
		'128v':[],
		'256v':[],
		'512v':[],
		'1024v':[],
		'16s':[],
		'32s':[],
		'64s':[],
		'128s':[],
		'256s':[],
		'512s':[],
		'1024s':[],
}
p64={
		'16p':[],
		'32p':[],
		'64p':[],
		'128p':[],
		'256p':[],
		'512p':[],
		'1024p':[],
		'16v':[],
		'32v':[],
		'64v':[],
		'128v':[],
		'256v':[],
		'512v':[],
		'1024v':[],
		'16s':[],
		'32s':[],
		'64s':[],
		'128s':[],
		'256s':[],
		'512s':[],
		'1024s':[],
}
P='16'
DP='16'
count= 0;
for line in file1:
	if line[0]=='P':
		P = line[3:5].strip()
		DP = line[17:22].strip()
		# print(P,DP)

	else:
		count+=1
		value = math.log(float(line.strip()))
		if P=='16':
			if count%3==1:
				p16[DP+'p'].append(value)
			if count%3==2:
				p16[DP+'v'].append(value)
			if count%3==0:
				p16[DP+'s'].append(value)

		if P=='36':
			if count%3==1:
				p36[DP+'p'].append(value)
			if count%3==2:
				p36[DP+'v'].append(value)
			if count%3==0:
				p36[DP+'s'].append(value)

		if P=='49':
			if count%3==1:
				p49[DP+'p'].append(value)
			if count%3==2:
				p49[DP+'v'].append(value)
			if count%3==0:
				p49[DP+'s'].append(value)

		if P=='64':
			if count%3==1:
				p64[DP+'p'].append(value)
			if count%3==2:
				p64[DP+'v'].append(value)
			if count%3==0:
				p64[DP+'s'].append(value)


file1.close() 


# ====================== Box Plot for P=16 =================================================
#fig = plt.figure(figsize =(8, 6)) 

# # Creating plot
x = [1,2,3,4,5,6,7]
plot16s = [p16['16s'],p16['32s'],p16['64s'],p16['128s'],p16['256s'],p16['512s'],p16['1024s']] 
mp16s = statistics.median(p16['16s'])
mp32s  = statistics.median(p16['32s'])
mp64s  = statistics.median(p16['64s'])
mp128s  = statistics.median(p16['128s'])
mp256s  = statistics.median(p16['256s'])
mp512s  = statistics.median(p16['512s'])
mp1024s  = statistics.median(p16['1024s'])
y1 = [mp16s, mp32s, mp64s,mp128s,mp256s,mp512s,mp1024s]
plt.boxplot(plot16s) 
plt.plot(x,y1,marker='o',label='Single send',color='green')

plot16p = [p16['16p'],p16['32p'],p16['64p'],p16['128p'],p16['256p'],p16['512p'],p16['1024p']] 
mp16p = statistics.median(p16['16p'])
mp32p  = statistics.median(p16['32p'])
mp64p  = statistics.median(p16['64p'])
mp128p  = statistics.median(p16['128p'])
mp256p  = statistics.median(p16['256p'])
mp512p  = statistics.median(p16['512p'])
mp1024p  = statistics.median(p16['1024p'])
y2 = [mp16p, mp32p, mp64p,mp128p,mp256p,mp512p,mp1024p]
plt.boxplot(plot16p) 
plt.plot(x,y2,marker='o',label='Packed send',color='red')


plot16v = [p16['16v'],p16['32v'],p16['64v'],p16['128v'],p16['256v'],p16['512v'],p16['1024v']] 
mp16v = statistics.median(p16['16v'])
mp32v  = statistics.median(p16['32v'])
mp64v  = statistics.median(p16['64v'])
mp128v  = statistics.median(p16['128v'])
mp256v  = statistics.median(p16['256v'])
mp512v  = statistics.median(p16['512v'])
mp1024v  = statistics.median(p16['1024v'])
y3 = [mp16v, mp32v, mp64v,mp128v,mp256v,mp512v,mp1024v]
plt.boxplot(plot16v) 
plt.plot(x,y3,marker='o',label='Derived Vector',color='blue')




plt.xticks([1,2,3,4,5,6,7],['16x16','32x32','64x64','128x128','256x256','512x512','1024x1024'])
plt.legend()
plt.title('Box plot for P=16')
plt.xlabel('Data Points(NxN)')
plt.ylabel('Time log(sec)')
# Save plot 
plt.savefig('plot16.png')
# plt.show() 
plt.clf()



# ====================== Box Plot for P = 36 =================================================
#fig = plt.figure(figsize =(8, 6)) 
# # Creating plot
x = [1,2,3,4,5,6,7]
plot36s = [p36['16s'],p36['32s'],p36['64s'],p36['128s'],p36['256s'],p36['512s'],p36['1024s']] 
mp16s = statistics.median(p36['16s'])
mp32s  = statistics.median(p36['32s'])
mp64s  = statistics.median(p36['64s'])
mp128s  = statistics.median(p36['128s'])
mp256s  = statistics.median(p36['256s'])
mp512s  = statistics.median(p36['512s'])
mp1024s  = statistics.median(p36['1024s'])
y1 = [mp16s, mp32s, mp64s,mp128s,mp256s,mp512s,mp1024s]
plt.boxplot(plot36s) 
plt.plot(x,y1,marker='o',label='Single send',color='green')

plot36p = [p36['16p'],p36['32p'],p36['64p'],p36['128p'],p36['256p'],p36['512p'],p36['1024p']] 
mp16p = statistics.median(p36['16p'])
mp32p  = statistics.median(p36['32p'])
mp64p  = statistics.median(p36['64p'])
mp128p  = statistics.median(p36['128p'])
mp256p  = statistics.median(p36['256p'])
mp512p  = statistics.median(p36['512p'])
mp1024p  = statistics.median(p36['1024p'])
y2 = [mp16p, mp32p, mp64p,mp128p,mp256p,mp512p,mp1024p]
plt.boxplot(plot36p) 
plt.plot(x,y2,marker='o',label='Packed send',color='red')


plot36v = [p36['16v'],p36['32v'],p36['64v'],p36['128v'],p36['256v'],p36['512v'],p36['1024v']] 
mp16v = statistics.median(p36['16v'])
mp32v  = statistics.median(p36['32v'])
mp64v  = statistics.median(p36['64v'])
mp128v  = statistics.median(p36['128v'])
mp256v  = statistics.median(p36['256v'])
mp512v  = statistics.median(p36['512v'])
mp1024v  = statistics.median(p36['1024v'])
y3 = [mp16v, mp32v, mp64v,mp128v,mp256v,mp512v,mp1024v]
plt.boxplot(plot36v) 
plt.plot(x,y3,marker='o',label='Derived Vector',color='blue')




plt.xticks([1,2,3,4,5,6,7],['16x16','32x32','64x64','128x128','256x256','512x512','1024x1024'])
plt.legend()
plt.title('Box plot for P=36')
plt.xlabel('Data Points(NxN)')
plt.ylabel('Time log(sec)')
# Save plot 
plt.savefig('plot36.png')
# plt.show() 
plt.clf()


# ====================== Box Plot for P = 49 =================================================

#fig = plt.figure(figsize =(8, 6)) 
# # Creating plot
x = [1,2,3,4,5,6,7]
plot49s = [p49['16s'],p49['32s'],p49['64s'],p49['128s'],p49['256s'],p49['512s'],p49['1024s']] 
mp16s = statistics.median(p49['16s'])
mp32s  = statistics.median(p49['32s'])
mp64s  = statistics.median(p49['64s'])
mp128s  = statistics.median(p49['128s'])
mp256s  = statistics.median(p49['256s'])
mp512s  = statistics.median(p49['512s'])
mp1024s  = statistics.median(p49['1024s'])
y1 = [mp16s, mp32s, mp64s,mp128s,mp256s,mp512s,mp1024s]
plt.boxplot(plot49s) 
plt.plot(x,y1,marker='o',label='Single send',color='green')

plot49p = [p49['16p'],p49['32p'],p49['64p'],p49['128p'],p49['256p'],p49['512p'],p49['1024p']] 
mp16p = statistics.median(p49['16p'])
mp32p  = statistics.median(p49['32p'])
mp64p  = statistics.median(p49['64p'])
mp128p  = statistics.median(p49['128p'])
mp256p  = statistics.median(p49['256p'])
mp512p  = statistics.median(p49['512p'])
mp1024p  = statistics.median(p49['1024p'])
y2 = [mp16p, mp32p, mp64p,mp128p,mp256p,mp512p,mp1024p]
plt.boxplot(plot49p) 
plt.plot(x,y2,marker='o',label='Packed send',color='red')


plot49v = [p49['16v'],p49['32v'],p49['64v'],p49['128v'],p49['256v'],p49['512v'],p49['1024v']] 
mp16v = statistics.median(p49['16v'])
mp32v  = statistics.median(p49['32v'])
mp64v  = statistics.median(p49['64v'])
mp128v  = statistics.median(p49['128v'])
mp256v  = statistics.median(p49['256v'])
mp512v  = statistics.median(p49['512v'])
mp1024v  = statistics.median(p49['1024v'])
y3 = [mp16v, mp32v, mp64v,mp128v,mp256v,mp512v,mp1024v]
plt.boxplot(plot49v) 
plt.plot(x,y3,marker='o',label='Derived Vector',color='blue')




plt.xticks([1,2,3,4,5,6,7],['16x16','32x32','64x64','128x128','256x256','512x512','1024x1024'])
plt.legend()
plt.title('Box plot for P=49')
plt.xlabel('Data Points(NxN)')
plt.ylabel('Time log(sec)')
# Save plot 
plt.savefig('plot49.png')
# plt.show() 
plt.clf()


# ====================== Box Plot for P = 64 =================================================


#fig = plt.figure(figsize =(8, 6)) 
# # Creating plot
x = [1,2,3,4,5,6,7]
plot64s = [p64['16s'],p64['32s'],p64['64s'],p64['128s'],p64['256s'],p64['512s'],p64['1024s']] 
mp16s = statistics.median(p64['16s'])
mp32s  = statistics.median(p64['32s'])
mp64s  = statistics.median(p64['64s'])
mp128s  = statistics.median(p64['128s'])
mp256s  = statistics.median(p64['256s'])
mp512s  = statistics.median(p64['512s'])
mp1024s  = statistics.median(p64['1024s'])
y1 = [mp16s, mp32s, mp64s,mp128s,mp256s,mp512s,mp1024s]
plt.boxplot(plot64s) 
plt.plot(x,y1,marker='o',label='Single send',color='green')

plot64p = [p64['16p'],p64['32p'],p64['64p'],p64['128p'],p64['256p'],p64['512p'],p64['1024p']] 
mp16p = statistics.median(p64['16p'])
mp32p  = statistics.median(p64['32p'])
mp64p  = statistics.median(p64['64p'])
mp128p  = statistics.median(p64['128p'])
mp256p  = statistics.median(p64['256p'])
mp512p  = statistics.median(p64['512p'])
mp1024p  = statistics.median(p64['1024p'])
y2 = [mp16p, mp32p, mp64p,mp128p,mp256p,mp512p,mp1024p]
plt.boxplot(plot64p) 
plt.plot(x,y2,marker='o',label='Packed send',color='red')


plot64v = [p64['16v'],p64['32v'],p64['64v'],p64['128v'],p64['256v'],p64['512v'],p64['1024v']] 
mp16v = statistics.median(p64['16v'])
mp32v  = statistics.median(p64['32v'])
mp64v  = statistics.median(p64['64v'])
mp128v  = statistics.median(p64['128v'])
mp256v  = statistics.median(p64['256v'])
mp512v  = statistics.median(p64['512v'])
mp1024v  = statistics.median(p64['1024v'])
y3 = [mp16v, mp32v, mp64v,mp128v,mp256v,mp512v,mp1024v]
plt.boxplot(plot64v) 
plt.plot(x,y3,marker='o',label='Derived Vector',color='blue')



plt.xticks([1,2,3,4,5,6,7],['16x16','32x32','64x64','128x128','256x256','512x512','1024x1024'])
plt.legend()
plt.title('Box plot for P=64')
plt.xlabel('Data Points(NxN)')
plt.ylabel('Time log(sec)')
# Save plot 
plt.savefig('plot64.png')
# plt.show() 
plt.clf()
