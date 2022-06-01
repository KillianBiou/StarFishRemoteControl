import json
import os
import subprocess
import shutil
from datetime import datetime

# This file is used to interface with the USB drive for saving the data folder.

# This function allows us to find if a USB drive is connected.
# It return the device name if it is connected, otherwise it returns None.
# Exemple: "F:\\"
def getUsbDrive():
    # Usage of powershell to get all connected devices and output the result as JSON.
    proc = subprocess.run(
        args=[
            'powershell',
            '-noprofile',
            '-command',
            'Get-WmiObject -Class Win32_LogicalDisk | Select-Object deviceid,volumename,drivetype | ConvertTo-Json'
        ],
        text=True,
        stdout=subprocess.PIPE
    )
    # We load the JSON file and iterate throught the recognized devices.
    for device in json.loads(proc.stdout):
        # If the device is of type 2 (Removables drive), we return the device name.
        if device['drivetype'] == 2:
            return device['deviceid']
    # If no device is found, we return None.
    return None

# This function copy the data folder to the USB drive with a timestamp.
def copyFile(src, usb):
    # We generate the timestamp (Ex: 20220511-16h46m45s).
    currentTime = datetime.now().strftime('%Y%m%d-%Hh%Mm%Ss')
    # We copy the data folder to the USB drive, ignoring the configuration folder (No reason to backup).
    shutil.copytree(src, usb + f'\\Data-{str(currentTime)}', ignore=shutil.ignore_patterns("configuration"))

    # We delete all the generated files from the data folder (Liberation of space)
    # The folder for the data logs.
    for f in os.listdir(os.path.join(src, 'dataLogs')):
        os.remove(os.path.join(src, 'dataLogs', f))
    # The folder for the images.
    for f in os.listdir(os.path.join(src, 'images', 'nonInterestClusters')):
        os.remove(os.path.join(src, 'images', "nonInterestClusters", f))
    for f in os.listdir(os.path.join(src, 'images', 'interestClusters')):
        os.remove(os.path.join(src, 'images', "interestClusters", f))