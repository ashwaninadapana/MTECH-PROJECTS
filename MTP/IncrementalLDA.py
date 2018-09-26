import csv

Reader = csv.reader(open("iris.csv","rb"),delimiter=",") #Read File in list of list
strX=[]
Data=[]
sum1=[]
i=0
for row in Reader:
    strX.append(row[:-1]) #Make a matrix containing only data values(i.e. except the labels)
    Data.append(row) #Make a copy of 'Reader' in matrix form
NT=len(strX)

#Convert strX to float values
for n in range(NT):
    for d in range(len(strX[n])):
        strX[n][d]=float(strX[n][d])

classes = set(map(lambda x1:x1[-1], Data)) #To get unique class labels from a column
sortedX = [[y[:-1] for y in Data if y[-1]==x1] for x1 in classes] #Group by class labels
meanC=[]

#Convert sortedX to float values
for c in range(len(sortedX)):
    for nc in range(len(sortedX[c])):
        for d in range(len(sortedX[c][nc])):
            sortedX[c][nc][d] = float(sortedX[c][nc][d])
M=len(sortedX)
meanX=[0]*len(sortedX[c][nc])


#Find class-wise mean
sumT=[0]*len(sortedX[c][nc])
for c in range(M):
    sum1=[0]*len(sortedX[c][nc])
    for nc in range(len(sortedX[c])):
        for d in range(len(sortedX[c][nc])):
            sum1[d]=sum1[d]+sortedX[c][nc][d]
            sumT[d]=sumT[d]+sortedX[c][nc][d]
    for d in range(len(sortedX[c][nc])):
        sum1[d]=sum1[d]/len(sortedX[c])					#Class mean
    meanC.append(sum1)
for d in range(len(sortedX[c][nc])):
    meanX[d]=sumT[d]/NT 								#Total mean
print "meanX:"
for d in range(len(sortedX[c][nc])):
    print meanX[d],
print "\n\nmeanC:"
for c in range(M):
    for d in range(len(sortedX[c][nc])):
        print meanC[c][d],


#Find Within Class Scatter Matrix for Traditional LDA
Sw = [[0 for x in range(len(sum1))] for y in range(len(sum1))]
Sw1 = [[0 for x in range(len(sum1))] for y in range(len(sum1))]

for c in range(M):
    for nc in range(len(sortedX[c])):
        sum1=[0]*len(sortedX[c][nc])
        for d in range(len(sortedX[c][nc])):
            sum1[d]=abs(sortedX[c][nc][d]-meanC[c][d])	#xnc-mean_xc
        for d1 in range(len(sum1)):
            for d2 in range(len(sum1)):
                Sw1[d1][d2]=sum1[d1]*sum1[d2] 			#sum1*sum1^T
        for d1 in range(len(sum1)):
            for d2 in range(len(sum1)):
                Sw[d1][d2]=Sw[d1][d2]+Sw1[d1][d2]		#Computing Sw

print "\n\nScatter Within Class:"
for d1 in range(len(sum1)):
    for d2 in range(len(sum1)):
        print Sw[d1][d2],
    print '\n'


#Find Between Class Scatter Matrix for Traditional LDA
Sb=[[0 for x in range(len(sum1))] for y in range(len(sum1))]
Sb1 = [[0 for x in range(len(sum1))] for y in range(len(sum1))]

for c in range(M):
    sum1=[0]*len(sortedX[c][nc])
    for d in range(len(sortedX[c][nc])):
        sum1[d]=abs(meanC[c][d]-meanX[d]) 					#meanC-meanX
    for d1 in range(len(sum1)):
        for d2 in range(len(sum1)):
            Sb1[d1][d2]=len(sortedX[c])*(sum1[d1]*sum1[d2])	#sum1*sum1^T
    for d1 in range(len(sum1)):
        for d2 in range(len(sum1)):
            Sb[d1][d2]=Sb[d1][d2]+Sb1[d1][d2]

print "Scatter Between Class:"
for d1 in range(len(sum1)):
    for d2 in range(len(sum1)):
        print Sb[d1][d2],
    print '\n'


#Sequential LDA
#Find Within Class Scatter Matrix for Sequential Incremental LDA
def Sequential():
    newXi=[]
    ip=raw_input("New Data Point with label: ")
    newXi = map(str, ip.split())

    if (newXi[-1] in classes): #Case-1 : New data point belongs to some existing class
        ind=classes.index(newXi[-1])
        ind_nc=len(sortedX[ind])
        for d in range(len(sortedX[ind][nc])):
            sortedX[ind][ind_nc][d]=float(newXi[d])
            meanC[ind][d]=(ind_nc*meanC[ind][d]+sortedX[ind][ind_nc][d])/(ind_nc+1)	#New classwise mean
            meanX[d]=(NT*meanX[d]+sortedX[M][0][d])/(NT+1) 	#Total mean
        print "New meanX:"
        for d in range(len(sortedX[c][nc])):
            print meanX[d],
        print "\n\nNew meanC:"
        for c in range(M):
            for d in range(len(sortedX[c][nc])):
                print meanC[c][d],

        #Find Within Class Scatter Matrix


        #Find Between Class Scatter Matrix
        Sb=[[0 for x in range(len(sum1))] for y in range(len(sum1))]
        Sb1 = [[0 for x in range(len(sum1))] for y in range(len(sum1))]

        for c in range(M):
            sum1=[0]*len(sortedX[c][nc])
            for d in range(len(sortedX[c][nc])):
                sum1[d]=abs(meanC[c][d]-meanX[d]) 				#newmeanC-newmeanX
            for d1 in range(len(sum1)):
                for d2 in range(len(sum1)):
                    Sb1[d1][d2]=(ind_nc+1)*(sum1[d1]*sum1[d2])	#sum1*sum1^T
            for d1 in range(len(sum1)):
                for d2 in range(len(sum1)):
                    Sb[d1][d2]=Sb[d1][d2]+Sb1[d1][d2]

        print "Scatter Between Class:"
        for d1 in range(len(sum1)):
            for d2 in range(len(sum1)):
                print Sb[d1][d2],
            print '\n'


    else: #Case-2 : New data point belongs to new class
        classes.append(newXi[-1])
        for d in range(len(sortedX[M-1][nc])):
            sortedX[M][0][d]=float(newXi[d])
            meanC[M][d]=sortedX[M][0][d]			#New classwise mean
            meanX[d]=(NT*meanX[d]+sortedX[M][0][d])/(NT+1) 	#Total mean
        print "New meanX:"
        for d in range(len(sortedX[c][nc])):
            print meanX[d],
        print "\n\nNew meanC:"
        for c in range(M):
            for d in range(len(sortedX[c][nc])):
                print meanC[c][d],

        #Within Class Scatter Matrix does not change in this case

        #Find Between Class Scatter Matrix
        Sb=[[0 for x in range(len(sum1))] for y in range(len(sum1))]
        Sb1 = [[0 for x in range(len(sum1))] for y in range(len(sum1))]

        for c in range(M+1):
            sum1=[0]*len(sortedX[c][nc])
            for d in range(len(sortedX[c][nc])):
                sum1[d]=abs(meanC[c][d]-meanX[d]) 					#meanC-newmeanX
            for d1 in range(len(sum1)):
                for d2 in range(len(sum1)):
                    Sb1[d1][d2]=len(sortedX[c])*(sum1[d1]*sum1[d2])	#sum1*sum1^T
            for d1 in range(len(sum1)):
                for d2 in range(len(sum1)):
                    Sb[d1][d2]=Sb[d1][d2]+Sb1[d1][d2]

        print "Scatter Between Class:"
        for d1 in range(len(sum1)):
            for d2 in range(len(sum1)):
                print Sb[d1][d2],
            print '\n'




#Chunk LDA
#Find Within Class Scatter Matrix for Chunk Incremental LDA
def Chunk():
    newfn = raw_input("New File Name: ")
    Reader = csv.reader(open(newfn,"rb"),delimiter=",")




#Select Mode: Sequential or Chunk
print "Select Mode: \n1.Sequential \n2.Chunk"
SorC = raw_input()
if (SorC=="1"):
    Sequential()
elif (SorC=="2"):
    Chunk()
