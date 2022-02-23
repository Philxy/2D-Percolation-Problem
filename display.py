import matplotlib.pyplot as plt
import retrieve_data
import os

p = ''
n = ''
for (rootDir, subDirs, files) in os.walk("."): 
    for file in files: 
        if file.startswith('largestCluster'):
            s = file.split()
            p = str(s[1])
            n = str(s[2])
            largest_cluster = retrieve_data.get_data_from_file("largestCluster " + str(p) + " " + str(n) +   " .txt")
            plt.imshow(largest_cluster, cmap='Blues')
            plt.title('n = ' + n + '   p = ' + p )
            plt.savefig("images/largestCluster " + str(p) + " " + str(n) +   " .png")
         




