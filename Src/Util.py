import os
import shutil
from enum import Enum

# This file contains some utility functions and enumerations that are used in the project

# This enumeration contains different types of message that the main program (StarFishScanner.py)
# and the configuration server (HTTPServer.py) can exchange
class ServerMessage(Enum):
    GENERATE_FORM = 1
    START_CAPTURE = 2
    STOP_CAPTURE = 3
    RESPONSE_SUCCESS = 4
    SWITCH_MODE = 5
    PARAMETERS_CHANGED = 6

# This enumeration contains the different types of states that the main program (StarFishScanner.py)
# can be in
class SFSState(Enum):
    IDLE = 0
    RUNNING = 1
    SAVING = 2
    SAVING_DONE = 3
    ERROR = 4

# This function read from the configuration file "parameters.conf" and returns a dictionary with those parameters
# "parameters.conf" need to be in the same folder as this utility file
def readParametersFile():
    paramDict = {}

    # We open the configuration file
    with open('parameters.conf', 'r') as f:
        lines = f.readlines()
        for line in lines:
            # We check to which parameter the line corresponds
            if line.startswith('TIME_SEPARATION'):
                # We split the line around the equal sign, we get the value get rid of extra spaces and new lines then we add it to the dictionary
                paramDict["timeseparation"] = line.split('=')[1].strip()
            elif line.startswith('OUTPUT_FOLDER'):
                # Same as before but some values are wrapped in quotes, so we need to get rid of those by ommiting the first and last character
                paramDict["outputfolder"] = line.split('=')[1].strip()[1:-1]
            elif line.startswith('CMAP'):
                paramDict["cmap"] = line.split('=')[1].strip()[1:-1]
            elif line.startswith('RANGE'):
                paramDict["range"] = line.split('=')[1].strip()
            elif line.startswith('GAIN'):
                paramDict["gain"] = line.split('=')[1].strip()
            elif line.startswith('CONTRAST'):
                paramDict["contrast"] = line.split('=')[1].strip()
            elif line.startswith('PORT'):
                paramDict["port"] = line.split('=')[1].strip()
            elif line.startswith('WAIT_TIME'):
                paramDict["restartWait"] = line.split('=')[1].strip()
            elif line.startswith('VOS'):
                paramDict["vos"] = line.split('=')[1].strip()
            elif line.startswith('SAMPLES'):
                paramDict["samples"] = line.split('=')[1].strip()
    # We return the dictionary
    return paramDict

def generateForm(webFolder, range, vos, samples, gain, contrast, timeseparation, cmap):
    with open(webFolder + '\\' + 'template.html', 'r') as f:
        data = f.read()
        data = data.replace('{RANGE}', range)
        data = data.replace('{VOS}', vos)
        data = data.replace('{SAMPLES}', samples)
        data = data.replace('{GAIN}', gain)
        data = data.replace('{CONTRAST}', contrast)
        data = data.replace('{CAPTURE_LENGTH}', timeseparation)
        data = data.replace(f'value="{cmap}"', f'value="{cmap}" selected')
    with open(webFolder + '/' + 'index.html', 'w+') as f:
        f.write(data)

# This function generate all the necessary folders to run StarFishScanner.py
# For now, it generates the following folders:
# data/ : where the data are stored
# data/dataLogs/ : where the CSV from the sonar are stored
# data/images/ : where the generated images are stored
# data/logs : where the running logs are stored
# data/configuration/ : where the necessary files to run the configuration interface are stored
def generateFolder(rootFolder):
    # We create all the folders, only if they dont exist
    if not os.path.exists(rootFolder):
        print('Creating root folder')
        os.mkdir(rootFolder)
    if not (os.path.exists(rootFolder + '/' + 'dataLogs')):
        print('Creating dataLogs folder')
        os.mkdir(rootFolder + '/' + 'dataLogs')
    if not (os.path.exists(rootFolder + '/' + 'images')):
        print('Creating images folder')
        os.mkdir(rootFolder + '/' + 'images')
    if not (os.path.exists(rootFolder + '/images/interestClusters')):
        print('Creating interest cluster images folder')
        os.mkdir(rootFolder + '/images/interestClusters')
    if not (os.path.exists(rootFolder + '/images/nonInterestClusters')):
        print('Creating non interest cluster images folder')
        os.mkdir(rootFolder + '/images/nonInterestClusters')
    if not (os.path.exists(rootFolder + '/' + 'logs')):
        print('Creating logs folder')
        os.mkdir(rootFolder + '/' + 'logs')
    if not (os.path.exists(rootFolder + '/' + 'configuration')):
        shutil.copytree("configuration", rootFolder + '/' + 'configuration')