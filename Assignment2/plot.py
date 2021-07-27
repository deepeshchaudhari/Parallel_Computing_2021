#!/usr/bin/env python
# coding: utf-8

# In[47]:

import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt

sns.set()

# In[48]:

Bcast_input_format = pd.DataFrame.from_dict({
    "D": [],
    "P": [],
    "ppn": [],
    "mode": [],  # 1 --> optimized, 0 --> standard
    "time": [],
})

Gather_input_format = pd.DataFrame.from_dict({
    "D": [],
    "P": [],
    "ppn": [],
    "mode": [],  # 1 --> optimized, 0 --> standard
    "time": [],
})

Reduce_input_format = pd.DataFrame.from_dict({
    "D": [],
    "P": [],
    "ppn": [],
    "mode": [],  # 1 --> optimized, 0 --> standard
    "time": [],
})

Alltoallv_input_format = pd.DataFrame.from_dict({
    "D": [],
    "P": [],
    "ppn": [],
    "mode": [],  # 1 --> optimized, 0 --> standard
    "time": [],
})


file1 = open("data.txt","r") 

for line in file1:
    value = line.split()
    #print(value)
    if len(value)==0:
        break

    D = int(value[1])
    ppn = int(value[2])
    P = int(value[3])*ppn
    time = float(value[4])


# ============================================================

    if value[0]=='Bcast':
        mode=0
        Bcast_input_format = Bcast_input_format.append({
        "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
    }, ignore_index=True)

    if value[0]=='OBcast':
        mode=1
        Bcast_input_format = Bcast_input_format.append({
            "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
        }, ignore_index=True)




# ============================================================

    if value[0]=='Gather':
        mode=0
        Gather_input_format = Gather_input_format.append({
        "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
    }, ignore_index=True)

    if value[0]=='OGather':
        mode=1
        Gather_input_format = Gather_input_format.append({
            "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
        }, ignore_index=True)




# ============================================================

    if value[0]=='Reduce':
        mode=0
        Reduce_input_format = Reduce_input_format.append({
        "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
    }, ignore_index=True)

    if value[0]=='OReduce':
        mode=1
        Reduce_input_format = Reduce_input_format.append({
            "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
        }, ignore_index=True)


# ============================================================

    if value[0]=='AlltoAllv':
        mode=0
        Alltoallv_input_format = Alltoallv_input_format.append({
        "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
    }, ignore_index=True)

    if value[0]=='OAlltoAllv':
        mode=1
        Alltoallv_input_format = Alltoallv_input_format.append({
            "D": D, "P": P, "ppn": ppn, "mode": mode, "time": time
        }, ignore_index=True)




print(Bcast_input_format)
Bcast_input_format["(P, ppn)"] = list(map(lambda x, y: ("(" + x + ", " + y + ")"), map(str, Bcast_input_format["P"]), map(str, Bcast_input_format["ppn"])))
sns.catplot(x="(P, ppn)", y="time", data=Bcast_input_format, kind="bar", col="D", hue="mode")
plt.savefig('plot_Bcast.png')

print(Gather_input_format)
Gather_input_format["(P, ppn)"] = list(map(lambda x, y: ("(" + x + ", " + y + ")"), map(str, Gather_input_format["P"]), map(str, Gather_input_format["ppn"])))
sns.catplot(x="(P, ppn)", y="time", data=Gather_input_format, kind="bar", col="D", hue="mode")
plt.savefig('plot_Gather.png')

print(Reduce_input_format)
Reduce_input_format["(P, ppn)"] = list(map(lambda x, y: ("(" + x + ", " + y + ")"), map(str, Reduce_input_format["P"]), map(str, Reduce_input_format["ppn"])))
sns.catplot(x="(P, ppn)", y="time", data=Reduce_input_format, kind="bar", col="D", hue="mode")
plt.savefig('plot_Reduce.png')

print(Alltoallv_input_format)
Alltoallv_input_format["(P, ppn)"] = list(map(lambda x, y: ("(" + x + ", " + y + ")"), map(str, Alltoallv_input_format["P"]), map(str, Alltoallv_input_format["ppn"])))
sns.catplot(x="(P, ppn)", y="time", data=Alltoallv_input_format, kind="bar", col="D", hue="mode")
plt.savefig('plot_Alltoallv.png')
