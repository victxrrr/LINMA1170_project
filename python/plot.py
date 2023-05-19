import numpy as np
import matplotlib.pyplot as plt

plt.rcParams["mathtext.fontset"] = 'cm'
plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 6
plt.rcParams['figure.autolayout'] = True
plt.rcParams['figure.dpi'] = 400
plt.rcParams['figure.figsize'] = [5, 3]
plt.rcParams['grid.linewidth'] = 0.6

colors = ['lightsteelblue', 'lightcoral', 'burlywood', 'thistle']
edgecolors = ['cornflowerblue', 'tomato', 'peru', 'violet']

data = np.loadtxt('../files/executionTimes.txt')
data[0][3] = 1000

# Largeur des barres
bar_width = 0.22
x = np.arange(1, 5)

# Création du graphique
b1 = plt.bar(x - bar_width * 1.5, data[0], width=bar_width, align='center', color = colors[0], edgecolor = edgecolors[0], linewidth=.6, label='Default')
b2 = plt.bar(x - bar_width * 0.5, data[1], width=bar_width, align='center', color = colors[1], edgecolor = edgecolors[1], linewidth=.6, label='Symmetry')
b3 = plt.bar(x + bar_width * 0.5, data[2], width=bar_width, align='center', color = colors[2], edgecolor = edgecolors[2], linewidth=.6, label='LAPACKE')
b4 = plt.bar(x + bar_width * 1.5, data[3], width=bar_width, align='center', color = colors[3], edgecolor = edgecolors[3], linewidth=.6, label='Symmetry + LAPACKE')

# Configuration des axes et de la légende
plt.xlabel('Facteur de maillage')
plt.ylabel('Temps d\'exécution (s)')
plt.xticks(x, ['1.0', '0.5', '0.25', '0.15'])
plt.yscale('log')
plt.legend()

fs = 5
h = lambda y : 1.1*y

for i in range(4):
    yval = b1[i].get_height()
    if i < 3:
        plt.text(b1[i].get_x()+bar_width/2, h(yval), f"{data[0][i]:.2f}", fontsize = fs, horizontalalignment='center')
    else:
        plt.text(b1[i].get_x()+bar_width/2, h(yval), r'$\infty$', fontsize = fs, horizontalalignment='center')
    yval = b2[i].get_height()
    plt.text(b2[i].get_x()+bar_width/2, h(yval), f"{data[1][i]:.2f}", fontsize = fs, horizontalalignment='center')
    yval = b3[i].get_height()
    plt.text(b3[i].get_x()+bar_width/2, h(yval), f"{data[2][i]:.2f}", fontsize = fs, horizontalalignment='center')
    yval = b4[i].get_height()
    plt.text(b4[i].get_x()+bar_width/2, h(yval), f"{data[3][i]:.2f}", fontsize = fs, horizontalalignment='center')

plt.show()