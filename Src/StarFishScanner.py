import os
import sqlite3
import time
import threading
from queue import Queue

import HTTPServer
import USBInterface
import Util
from Util import SFSState, ServerMessage
from Process import Process

# This file contains the main logic for the StarFishScanner.

# This class is the pilot all the other classes in the program.
class SFSController:
    def __init__(self):
        # Generation of base attributes
        self.state = SFSState.IDLE
        self.serverIn = Queue()
        self.serverOut = Queue()
        self.grafanaIn = Queue()
        self.grafanaOut = Queue()
        self.generateDatabase()
        #We fetch the parameters from the parameters file
        self.loadParameters()
        # We start a thread for the configuration server as a deamon (Ending the thread when the program ends)
        self.serverDaemon = threading.Thread(target=self.startServer)
        self.serverDaemon.daemon = True
        self.serverDaemon.start()

        self.grafanaDaemon = threading.Thread(target=self.startGrafanaServer)
        self.grafanaDaemon.daemon = True
        self.grafanaDaemon.start()

    def generateDatabase(self):
        self.sqliteConnection = sqlite3.connect("database.db")
        self.sqliteConnection.execute("DROP TABLE IF EXISTS SFParameters")
        self.sqliteConnection.execute("CREATE TABLE SFParameters (name, value)")
        self.sqliteConnection.execute("DROP TABLE IF EXISTS VisualisationParameters")
        self.sqliteConnection.execute("CREATE TABLE VisualisationParameters (name, value)")
        self.sqliteConnection.execute("DROP TABLE IF EXISTS MiscParameters")
        self.sqliteConnection.execute("CREATE TABLE MiscParameters (name, value)")
        self.sqliteConnection.execute("DROP TABLE IF EXISTS RuntimeVariable")
        self.sqliteConnection.execute("CREATE TABLE RuntimeVariable (name, value)")

        self.sqliteConnection.execute("INSERT INTO RuntimeVariable VALUES ('lastImagePath', '')")
        self.sqliteConnection.commit()


    # This method loads the parameters from the parameters.conf file
    def loadParameters(self):
        # We use the Util file to read the file and receive a dictionary with the parameters
        params = Util.readParametersFile()
        # We process all the parameters
        self.outputFolder = params['outputfolder']
        # We use the Util file to generate all the folders arborescence needed, starting from the specified output folder
        Util.generateFolder(self.outputFolder)
        # Remember all the needed folders
        self.logsFolder = os.path.join(self.outputFolder, 'logs')
        self.dataLogsFolder = os.path.join(self.outputFolder, 'dataLogs')
        self.imagesFolder = os.path.join(self.outputFolder, 'images')
        self.webFolder = os.path.join(self.outputFolder, 'configuration')
        # Remember the length of one CSV file
        self.timeseparation = params['timeseparation']
        # Remember the chosen color map
        self.cmap = params['cmap']
        # Remember the port for the configuration server
        self.port = int(params['port'])
        # Remember the waiting time before restarting the capture after saving
        self.waitTime = float(params['restartWait'])
        # Remember the parameters of the StarFish
        self.range =  params['range']
        self.vos = params['vos']
        self.samples = params['samples']

        self.contrast = params['contrast']
        self.gain = params['gain']

        self.sqliteConnection.execute("INSERT INTO SFParameters VALUES ('state', ?)", (0,))
        self.sqliteConnection.execute("INSERT INTO SFParameters VALUES ('range', ?)", (int(self.range),))
        self.sqliteConnection.execute("INSERT INTO SFParameters VALUES ('samples', ?)", (int(self.samples),))
        self.sqliteConnection.execute("INSERT INTO SFParameters VALUES ('vos', ?)", (int(self.vos),))

        self.sqliteConnection.execute("INSERT INTO VisualisationParameters VALUES ('gain', ?)", (int(self.gain),))
        self.sqliteConnection.execute("INSERT INTO VisualisationParameters VALUES ('contrast', ?)", (int(self.contrast),))

        self.sqliteConnection.execute("INSERT INTO MiscParameters VALUES ('timeseparation', ?)", (int(self.timeseparation),))
        self.sqliteConnection.execute("INSERT INTO MiscParameters VALUES ('cmap', ?)", (self.cmap,))

        self.sqliteConnection.commit()

    def refreshParameters(self):
        self.range = str(self.sqliteConnection.execute("SELECT value FROM SFParameters where name = 'range';").fetchone()[0])
        self.vos = str(self.sqliteConnection.execute("SELECT value FROM SFParameters where name = 'vos';").fetchone()[0])
        self.samples = str(self.sqliteConnection.execute("SELECT value FROM SFParameters where name = 'samples';").fetchone()[0])
        self.contrast = str(self.sqliteConnection.execute("SELECT value FROM VisualisationParameters where name = 'contrast';").fetchone()[0])
        self.gain = str(self.sqliteConnection.execute("SELECT value FROM VisualisationParameters where name = 'gain';").fetchone()[0])
        self.timeseparation = str(self.sqliteConnection.execute("SELECT value FROM MiscParameters where name = 'timeseparation';").fetchone()[0])
        self.cmap = str(self.sqliteConnection.execute("SELECT value FROM MiscParameters where name = 'cmap';").fetchone()[0])

    # This method start the two subprocesses (SonarMode : Data capture and Visualize : Image Generation)
    def startProcesses(self):
        # Generation of the command in the form of a list (Needed for POpen)]
        sonarModeParams = ["SonarMode.exe", self.timeseparation, self.range, self.samples,
                           self.vos, self.gain, self.contrast, self.dataLogsFolder]
        visualizeParams = ["python", "-u", "Visualize.py", self.dataLogsFolder, self.imagesFolder, self.cmap]

        # We make use of the Process class to start wrap the subprocesses
        self.SONAR_MODE_PROCESS = Process("SonarMode", sonarModeParams, os.path.join(self.logsFolder, "sonarModLog.txt"))
        self.VISUALIZE_PROCESS = Process("Visualize", visualizeParams, os.path.join(self.logsFolder, "visualizeLog.txt"))

        # We start the two subprocesses
        self.SONAR_MODE_PROCESS.run()
        self.VISUALIZE_PROCESS.run()
        # The capture is now running
        self.state = SFSState.RUNNING

    # Stop the two subprocesses (SonarMode : Data capture and Visualize : Image Generation)
    def stopProcesses(self):
        self.SONAR_MODE_PROCESS.stop()
        self.VISUALIZE_PROCESS.stop()
        tempConnection = sqlite3.connect("database.db")
        tempConnection.execute("UPDATE SFParameters SET value = 0 WHERE name = 'state'")
        tempConnection.commit()

    # This method is the main loop of the program with events catching
    def run(self):
        # At start, we start the two subprocesses
        self.startProcesses()
        while True:
            # We look for an event
            self.catchEvents()
            # We wait a little bit to prevent the overusage of the CPU and battery
            time.sleep(0.1)

    # This method start the configuration server
    # ADVICE : Call this method with a thread to avoid blocking the main loop
    def startServer(self):
        # Usage of the DataHTTPServer class to wrap the configuration server
        self.server = HTTPServer.DataHTTPServer(self.outputFolder, self.port, self.serverIn, self.serverOut)
        self.server.start()
        print("Server serving...")

    def startGrafanaServer(self):
        # Usage of the DataHTTPServer class to wrap the configuration server
        self.grafanaServer = HTTPServer.GrafanaDataHTTPServer(self.outputFolder, self.port + 1, self.grafanaIn, self.grafanaOut)
        self.grafanaServer.start()
        print("Server serving...")

    # This method generate the Board
    def generateForm(self):
        Util.generateForm(self.webFolder, self.range, self.vos, self.samples, self.gain, self.contrast, self.timeseparation, self.cmap)

    # This method catch events (See Util.ServerMessage)
    def catchEvents(self):
        # We check the current state of the capture
        self.refreshProcessesState()
        # We check if we have a message from the server
        if not self.serverOut.empty():
            print("Message from server")
            message = self.serverOut.get()
            # If he ask us to generate the control board
            if message == ServerMessage.GENERATE_FORM:
                # We generate the board
                print("Generating form...")
                self.generateForm()
                print("Form generated")
                # We send a success message to the server
                self.serverIn.put(ServerMessage.RESPONSE_SUCCESS)
                print("Success message sent")
            # If the parameters has been changed
            elif message == ServerMessage.PARAMETERS_CHANGED:
                self.refreshParameters()
                self.serverIn.put(ServerMessage.RESPONSE_SUCCESS)
                if self.state == SFSState.RUNNING:
                    self.stopProcesses()
                    time.sleep(1)
                    self.startProcesses()

        if not self.grafanaOut.empty():
            print("Grafana message received")
            message = self.grafanaOut.get()
            # If he ask us to generate the control board
            if message == ServerMessage.SWITCH_MODE:
                print("SWITCH MODE")
                if self.state == SFSState.RUNNING:
                    self.stopProcesses()
                elif self.state == SFSState.IDLE or self.state == SFSState.ERROR:
                    print("STARTING")
                    self.startProcesses()
                self.grafanaIn.put(ServerMessage.RESPONSE_SUCCESS)

        # While in the specified state, we can't go further
        if self.state == SFSState.SAVING or self.state == SFSState.SAVING_DONE:
            return
        # If we detect a removable disk then we save the data
        if self.usbConnected():
            # We go in saving state
            self.state = SFSState.SAVING
            # We call the save method in a thread to avoid blocking the main loop (Generation of the board)
            usbSaveThread = threading.Thread(target=self.exportToUsb)
            usbSaveThread.start()

    # This method refresh the state of the capture
    def refreshProcessesState(self):
        # While saving or removable disk is connected, we don't change the state (Prevention of scary events)
        if self.state == SFSState.SAVING or self.state == SFSState.SAVING_DONE:
            return # do not refresh state while saving
        # If both subprocess are running, RUNNING state, no problem
        if self.SONAR_MODE_PROCESS.checkState() == None and self.VISUALIZE_PROCESS.checkState() == None:
            self.state = SFSState.RUNNING
        # If both subprocess are not running, IDLE state, no problem
        elif self.SONAR_MODE_PROCESS.checkState() is not None and self.VISUALIZE_PROCESS.checkState() is not None:
            self.state = SFSState.IDLE
        # If one of the subprocess is running, but not the other, we have a problem, ERROR state
        else:
            self.state = SFSState.ERROR

    # This metho save the data to the removable disk
    def exportToUsb(self):
        print("USB drive connected")
        # We fetch the removable disk name
        usb = USBInterface.getUsbDrive()
        # We stop the processes to avoid data corruption
        self.stopProcesses()
        tempConnection = sqlite3.connect("database.db")
        tempConnection.execute("UPDATE SFParameters SET value = -2 WHERE name = 'state'")
        tempConnection.commit()
        # We copy the data
        USBInterface.copyFile(self.outputFolder, usb)
        # We are done saving
        self.state = SFSState.SAVING_DONE
        tempConnection.execute("UPDATE SFParameters SET value = -3 WHERE name = 'state'")
        tempConnection.commit()
        # We wait for the user to remove the usb drive
        while os.path.exists(usb):
            print("Waiting for USB drive to be removed...")
            time.sleep(1)
        # We are now IDLE and we can start again
        self.state = SFSState.IDLE
        tempConnection.execute("UPDATE SFParameters SET value = 0 WHERE name = 'state'")
        tempConnection.commit()
        # We wait the specified time before starting again
        time.sleep(self.waitTime)
        # We check if the client restarted the capture with the board, if not, we restart the subprocesses
        if self.state != SFSState.RUNNING:
            self.startProcesses()

    # This methos return true if a removable disk is connected, false otherwise
    def usbConnected(self):
        usb = USBInterface.getUsbDrive()
        if usb is not None:
            return True
        return False

if __name__ == '__main__':
    # We create the controller and run it
    controller = SFSController()
    controller.run()
