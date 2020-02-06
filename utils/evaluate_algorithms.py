import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


columns = ['length = 8', 'length = 16', 'length = 32', 'length = 64']
algorithms = open('algorithms.txt', 'r').read().splitlines()
data = pd.read_csv('all8-64.csv', names=columns, sep='\t', lineterminator='\n')


# data['mean'] = data[['plen8', 'plen16']].mean(axis=1)
data['mean'] = data.mean(axis=1)
top5 = data.sort_values('mean').head(5)

algorithms = open('algorithms.txt', 'r').read().splitlines()

top5_algos = [algorithms[i] for i in top5.index]
print(top5_algos)
# top5.index.names = [1, 2, 3, 4, 5]
x = top5.index.values
x = top5_algos
# print(type(top5.index))
# top5.reindex(top5_algos, copy=False, axis=0)
# top5.insert(loc=0, column='Algorithm', value=top5_algos)
# for i in top5.index:
# 	print(i)
print(top5)

plt.rc('axes', axisbelow=True)

top5.iloc[:, :-1].plot.bar(cmap='plasma')
plt.legend(loc=2, prop={'size': 21})
plt.xticks(np.arange(5), top5_algos, rotation=0, fontsize=23)
plt.yticks(fontsize=23)
plt.xlabel('Algorithms', fontsize=23)
plt.ylabel('Time (s)', fontsize=23)
# plt.savefig('best5.png')
# fig, axs = plt.subplots(2, 2)

# top5['len8'].plot.bar(ax=axs[0,0], color='yellow')
# top5['len32'].plot.bar(ax=axs[1,0], color='yellow')
# top5['len16'].plot.bar(ax=axs[0,1], color='yellow')
# top5['len64'].plot.bar(ax=axs[1,1], color='yellow')

# # plt.xticks(np.arange(5), top5_algos)
# axs[0,0].set_xticks(np.arange(5), top5_algos)

# # axs[1,1].set_xticks(np.arange(5), columns)



# for ax in axs.flat:
# 	ax.set_xticks(np.arange(5), top5_algos)
# 	ax.set(xlabel='Algorithms', ylabel='Time(s)')
# 	ax.label_outer()
# # Hide x labels and tick labels for top plots and y ticks for right plots.
plt.show()
# axs[0, 0].plot(x, y)
# axs[0, 0].set_title('Axis [0,0]')
# axs[0, 1].plot(x, y, 'tab:orange')
# axs[0, 1].set_title('Axis [0,1]')
# axs[1, 0].plot(x, -y, 'tab:green')
# axs[1, 0].set_title('Axis [1,0]')
# axs[1, 1].plot(x, -y, 'tab:red')
# axs[1, 1].set_title('Axis [1,1]')

# for ax in axs.flat:
#     ax.set(xlabel='x-label', ylabel='y-label')

# # Hide x labels and tick labels for top plots and y ticks for right plots.
# for ax in axs.flat:
#     ax.label_outer()
