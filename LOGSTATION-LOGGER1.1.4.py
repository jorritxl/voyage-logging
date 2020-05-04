import serial # import Serial Library
import numpy  # Import numpy
import datetime
import time
import csv

timestamprt = str(datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H%M%S'))
timestamprt = str(timestamprt)

now = datetime.datetime.now()
arduinoData = serial.Serial('com14', 9600) #Creating our serial object named arduinoData
cnt=1
row = ['String', ' String']

##with open('logone.csv', 'r') as readFile:
##    reader = csv.reader(readFile)
##    lines = list(reader)
##    lines.insert(1, row)
    
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    timestamp = str(datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H:%M:%S'))
    line = arduinoData.readline()          #read the line of text from the serial port
    leknn = str(line, 'utf-8')              #byte to str
    lekn = leknn +"," + timestamp
    
    dataArray = lekn.split(',')            #Split it into an array called dataArray
    rpm = float( dataArray[0])            #Convert first element to floating number and put in temp
    gpslat =    float( dataArray[1])            #Convert second element to floating number and put in P
    gpslng =    float( dataArray[2])
    gpssog = ( dataArray[3])
    gpssats = ( dataArray[4])
    gpshdop = ( dataArray[5])
    tijd = ( dataArray[6])



                        #Call drawnow to update our live graph
    
    cnt=cnt+1
    print(cnt)

    with open(timestamprt+'shiplog.csv', 'a', newline='') as csvfile:
            spamwriter = csv.writer(csvfile, delimiter=' ', quoting=csv.QUOTE_MINIMAL)
            spamwriter.writerow([rpm]+[","]+[gpslat]+[","]+[gpslng]+[","]+[tijd])


