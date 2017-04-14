import sys


mem=sys.argv[1]

mem2=mem.split(" ")
sum=0
lenn=0
for i in mem2:
	sum = sum + float(i)
	if (float(i)!=0):
		lenn = lenn +1



print (sum/(lenn))*10.5/1000

