import serial
import csv
import time
from datetime import datetime

### PROGRAM PARAMETERS ###
print_labels = True #if we want column headers for each sensor reading type (pressure, temperature etc) (True/False)
timestamp = True #if we want to timestamp our readings (True/False)
baud = 9600 #arduino uno runs at 9600 baud
arduino_port = input("Enter serial port of Arduino (eg COM4 on Windows): ") #serial port of Arduino (eg: COM4 on windows)
while True:
    try:
        ser = serial.Serial(arduino_port, baud)
    except:
        arduino_port = input("Invalid input, please enter serial port of Arduino: ")
        continue
    break
    
fileName = input("Enter file name of spreadsheet to create/log to (today's date will be suffixed): ") #name of the CSV file generated
fileName += str(datetime.now().date())
fileName += ".csv"

INF = False #if true, the sensor will keep logging readings indefinitely till it is unplugged
durList = input("Enter (in format HRS MINS SECS) how long you want to log data for (simply press enter if you wish to log indefinitely): ").split() #how many samples to collect
dur = 0
while True:
    if len(durList) == 0:
        INF = True
        break
    try:
        if len(durList) != 3 or len([s for s in durList if s.isdigit()]) != 3:
            #print(len(durList), len([s for s in durList if s.isdigit()]), [s for s in durList if s.isdigit()])
            raise Exception("e")
        dur = int(durList[0]) * 3600 + int(durList[1]) * 60 + int(durList[2])
    except:
        durList = input("Please enter in format HRS MINS SECS (eg 1 2 3): ").split()
        continue
    break

# Wait for serial monitor to reconnect, and dump garbage data in it
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
time.sleep(5) #Give arduino time to reconnect
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "w", newline='')
print("File:", fileName, "created.")

line = 0
startTime = time.time()
endTime = startTime + dur

def timeElapsed():
    """ Returns the time elapsed in the sensor taking readings in a nice format """
    res = time.time() - startTime
    hours = int(res / 3600)
    res -= int(res / 3600) * 3600
    minutes = int(res / 60)
    res -= int(res / 60) * 60
    res = round(res, 3)
    return str(hours) + " hrs " + str(minutes) + " mins " + str(res) + " secs."
    
try:
    while time.time() < endTime or INF:
        if print_labels and line == 0:
            print("Printing Column Headers")
            col_header = ["Partial Pressure O2 / mbar", "Temperature / °C", "Pressure / mbar", "Concentration O2 / %"]
            if timestamp == True:
                col_header.insert(0, "Reading Timestamp")
            file_writer = csv.writer(file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            file_writer.writerow(col_header)
            line += 1
        else:
            if line == 0:
                line = 1;
            print("Line " + str(line) + ": writing...")
            getData = str(ser.readline())
            data = getData[2:][:-5]
        
            print(data)
            str_list = data.split(" ")
            snsr_readings = [str_list[1], str_list[3], str_list[5], str_list[7]]
            print("%O2:", str_list[7])
            if timestamp == True:
                dateTimeObj = datetime.now()
                timestampStr = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")
                snsr_readings.insert(0, timestampStr)
            file_writer = csv.writer(file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            file_writer.writerow(snsr_readings)
            line += 1
except:
    # Catch exception if Arduino disconnceted from computer before all readings were in
    print("Arduino disconnected!")
    
print("Done logging.", line - 1, "readings were logged to", fileName, "Time elapsed:", timeElapsed())
file.close()
