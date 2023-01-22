import subprocess


domains = ['utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','46.188.47.18','82.71.8.205','212.102.51.18','43.229.60.176','210.5.56.145']
# UT Austin, Google, Califorina, New York, Toronto Canada, South Africa, Moscow, London, Tokyo, Sydney Australia, Henan China

x = 0
while x < len(domains):
    subprocess.run(['>' + domains[x] + '-traces.txt'],shell=True)
    x = x + 1

for i in range(10000):
    j = 0
    while j < len(domains):

        date = subprocess.run(['date','+%m-%d-%X'],capture_output=True,text=True)
        dateString = date.stdout
        dateString = dateString.rstrip()
        subprocess.run(["echo === " + domains[j] + '-' + dateString + '-' + 'TraceNumber-' + str(i) + ' === >> ' + domains[j] + '-traces.txt'],shell=True)
        subprocess.run(['traceroute -n -m 45 -A ' + domains[j] + ' >> ' + domains[j] + '-traces.txt'],shell=True)
        j = j + 1
 
