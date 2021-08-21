# COSIFE - Optical Oxygen Sensor Project
*Cost-effective Oxygen Sensors In Flexible Environments*

This project uses the LuminOx Optical Oxygen Sensor (model: LOX-02), controlled by the arduino electronics platform. Sensor readings can be logged to the computer, either directly to the serial monitor, or in a timestamped and organized format to a csv file (recommended). Readings are also displayed on the onboard OLED screen. The option of having the audio-visual alarm being sounded once percent oxygen concentration exceeds a certain threshold is also available.

The LuminOx Optical Oxygen Sensor family functions based off of the principle of fluorescence quenching by oxygen, which gives it numerous unique advantages, such as minimal cross sensitivity to other gases, high accuracy, factory calibrated (no need to calibrate afterwards), and long working lifespan.

## How to install dependancies
### Python module dependancies:
* pyserial
* serial

NB: [Python 3](https://www.python.org/) needs to be installed on your system for `log_to_csv.py` to work, which governs logging and formatting sensor readings to a spreadsheet. To install the above python modules, run ```pip install {module}``` in the command line. Note that you might have to navigate to the ```Scripts``` folder in your python installation folder to run this command if ```pip``` is not in your environment variables.

### Arduino module dependancies:
* pitches.h
* adafruit_SSD1306.h
* adafruit_GFX.h

NB: [Arduino IDE](https://www.arduino.cc/) needs to be installed on your system to set the oxygen concentration threshold directly onto the arduino board, and to log sensor readings in the serial monitor. In Arduino IDE, go to ```Sketch > Include Library > Manage Libraries >``` and search for the latter two modules above. Searching for keywords like ```1306``` and ```GFX``` might be easier. Click install, and click ```Install All``` when it asks if you want to install all missing dependancies. For the first arduino module, download the zip file from this repository, and go to ```Sketch > Include Library > Add .ZIP Library``` and navigate to your downloaded ```pitches.zip``` zip file.

## How to use
To set the oxygen concentration threshold for the alarm, and the alarm mode, go to `LOX-02/LOX-02.ino` and modify the constants at the top of the code under IMPORTANT CONSTANTS. Then click upload with the arduino plugged in to upload the new code to the arduino board.

To log sensor data to the computer, you have 2 options (the first one is preferred as it formats and timestamps the data into an organized spreadsheet and allows you to specify duration to log for, in addition to error checking and failsafes)
1. LOG TO .CSV file (spreadsheet) USING `log_to_csv.py`: Plug the arduino into the computer via USB first. Then run the python program to log sensor readings to a spreadsheet. You will be asked to provide certain inputs such as which port the arduino is on (```Tools > Port > Serial ports```), and how long to log for.
2. LOG TO ARDUINO SERIAL MONITOR: To log data to the serial monitor, simply open the serial monitor in your arduino IDE after you plugged the arduino into the computer via USB.

A video demonstration of the datalogging procedure for both options can be found here: https://youtu.be/V4u3jIYt8zI

If you wish to run the sensor as a standalone unit, connect the 9V battery to the DC power jack.
