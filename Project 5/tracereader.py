import re
import linecache
import ipinfo
domains = ['utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','46.188.47.18','82.71.8.205','212.102.51.18','43.229.60.176','210.5.56.145']

contents = []
linenum = 0 
pattern = re.compile("===")
matches = []
separatorLineNumbers = []
blocksOfText = []
access_token = '25d7a3f46995bd'
handler = ipinfo.getHandler(access_token)

# Read the contents of the files, using the === characters as a reference to know when a new trace started.
with open('C:/Users/jctol/Downloads/College classes/2022/CSDS 325/Project 5/icir.org-traces.txt') as f:
    for line in f:
        linenum += 1
        if pattern.search(line) != None:
            matches.append((linenum,line.rstrip('\n')))
            separatorLineNumbers.append(linenum)
        contents.append(line.rstrip('\n'))

# Store just the trace routes into a list called, blocksOfText
for i in range(0, len(separatorLineNumbers) - 1):
    blocksOfText.append("")
    for x in range(separatorLineNumbers[i] + 1, separatorLineNumbers[i + 1]):
        blocksOfText[i] += linecache.getline('C:/Users/jctol/Downloads/College classes/2022/CSDS 325/Project 5/icir.org-traces.txt', x)


# Output only the AS numbers for a specific route to a file
outFile = open('AS-numbers-icir.org-traces.txt','w')
count = 0
for k in range(0,len(blocksOfText) - 1):
    count = count + 1
    result = re.findall('(?<=\[)(.*?)(?=\])',blocksOfText[k])
    stringList = result
    hopCount = len(stringList)
    string = ' '.join(stringList)
    words = string.split()
    print(", ".join(sorted(set(words),key = words.index)), file = outFile) 



# Idea here is to print out only unique traces

outFile = open('Unique-routes-icir.org-traces.txt','w')
outFile2 = open('icir.org-coords.txt','w')
count = 0
stringList.clear()
coordList = []
finCoordList = []

for k in range(0,len(blocksOfText) - 1):
    count = count + 1
    innerCount = 0
    # Break the block into its individual lines
    lines = blocksOfText[k].split("\n")
    traceRoute = ""
    coords = ""
 
    # Check every string in a line to see if its an IP
    for word in lines:
        
        result = re.search("\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}",word)
        #If the word is an
        if result:
            # Found an IP, now geolocate it using ipinfo library
            ip = result.group()
            details = handler.getDetails(ip)
            # Try to see if the router is happy to give their location
            try:
                coords = str(details.loc) + "\n"
                coordList.insert(innerCount,coords)
            except(AttributeError):
                pass   
            # This should store all the IPs into one string, 
            traceRoute += ip + " "
        innerCount = innerCount + 1
    # Append all traces to one list
    stringList.append(traceRoute)
    test = "".join(coordList)
    print(test,file=outFile2)
    finCoordList.insert(k,test)
    
    # print(test)
    coordList.clear()
# Make the list a set so only the unique traces are left
uniqueSet = set(stringList)
numOfUnique = len(uniqueSet)
string = '\n'.join(uniqueSet)

stringList.clear()
print(string,file = outFile)





string = ''.join([str(elem) for elem in finCoordList])
print(string,file = outFile)

# for x in range(0,len(finCoordList) - 1):
#     coordList.insert(x,finCoordList)
# # print(coordList,file = outFile)
# print(coordList[0])

# Get only the ms from the trace routes
count  = 0
outFile = open('MS-of-trace-icir.org-traces.txt','w')
stringList.clear()
for k in range(0,len(blocksOfText) - 1):
    count = count + 1
    lines = blocksOfText[k].split("\n")
    finalString = ""

   #  print("\n".join(lines))
    for item in lines:
        result = re.search('\d{1,3}.\d{1,4} ms',item)
        if result:
            resultString = result.group()
            finalString += resultString
    print(finalString,file = outFile)




count = 0
outFile = open('hop-count-icir.org-traces.txt','w')
for k in range(0,len(blocksOfText) - 1):
    count = count + 1
    hopCount = blocksOfText[k].count('\n') - 1
    print(str(hopCount), file = outFile)
    
