import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np



data = np.loadtxt('Outfile.csv', delimiter=';', skiprows=0)
t=data[:,2]
i=data[:, 1]
u=data[:,3]
plt.subplot(2,1,1)
plt.plot(t,i, 'r')
plt.ylabel('Strom in A')
plt.xlabel('Zeit in s')
plt.title('Stromverlauf')
plt.subplot(2,1,2)
plt.plot(t,u, 'b')
plt.ylabel('Spannung in V')
plt.xlabel('Zeit in s')
plt.title('Spannungsverlauf')
plt.tight_layout()
print (data)
plt.show()


