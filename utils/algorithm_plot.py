import numpy as np
import matplotlib.pyplot as plt

qf43 = [29.6488, 28.8017, 29.4792, 30.9842]
bndm = [7.059, 7.03139, 8.47803, 7.16593]

y = np.arange(4)
# y = [8, 16, 32, 64]

plt.plot(y, qf43, label="QF43", linewidth=5)
plt.plot(y, bndm, label="SBNDMq4", linewidth=5)
plt.legend(loc='center right', prop={'size': 21})
plt.grid(True, linestyle=':')
plt.xticks(y, ('8', '16', '32', '64'), fontsize=23)
plt.yticks(fontsize=23)
plt.xlabel('Pattern Length', fontsize=23)
plt.ylabel('Time (s)', fontsize=23)
plt.show()

qf43_trim = [33.0342, 34.2925, 42.9897, 33.6567]
bndm_trim = [13.3473, 12.99992, 13.932, 13.854]

plt.plot(y, qf43_trim, label="QF43 w/ quality trim", linewidth=5)
plt.plot(y, bndm_trim, label="SBNDMq4 w/ quality trim", linewidth=5)
plt.legend(loc='center right', prop={'size': 21})
plt.grid(True, linestyle=':')
plt.xticks(y, ('8', '16', '32', '64'), fontsize=23)
plt.yticks(fontsize=23)
plt.xlabel('Pattern Length', fontsize=23)
plt.ylabel('Time (s)', fontsize=23)
plt.show()

# Tools

# Single End w/ Trim
all_tools_single = [8.11, 10.0, 54.0, 7.8]
#Paired End w/ Trim
all_tools_paired = [16.03, 16.3, 22.8, 108.1]

y = np.arange(len(all_tools_single))
plt.bar(y, all_tools_single, linewidth=2, color='grey')
for index,data in enumerate(all_tools_single):
    plt.text(x=index-.075 , y =data-4 , s=f"{data}" , fontdict=dict(fontsize=16), color='white')
# plt.legend(loc=2, prop={'size': 15})
plt.xticks(y, ('FAIR', 'Trimmomatic', 'Alien Trimmer', 'Adapter Removal'), fontsize=14)
plt.yticks(fontsize=14)
plt.xlabel('Tools', fontsize=15)
plt.ylabel('Time (s)', rotation=0, fontsize=14)
plt.show()

plt.bar(y, all_tools_paired, linewidth=2, color='grey')
for index,data in enumerate(all_tools_paired):
    plt.text(x=index-.1 , y =data-5 , s=f"{data}" , fontdict=dict(fontsize=21), color='white')
# plt.legend(loc=2, prop={'size': 15})
plt.xticks(y, ('FAIR', 'Adapter Removal', 'Trimmomatic', 'AlienTrimmer'), fontsize=23)
plt.yticks(fontsize=23)
plt.xlabel('Tools', fontsize=23)
plt.ylabel('Time (s)', fontsize=23)
plt.show()
