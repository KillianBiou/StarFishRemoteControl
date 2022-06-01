import subprocess

# Class that represent a process
class Process():
    # Constructor
    # name: name of the process (only for debug)
    # params: the command to run the process (Ex : ["python3", "main.py", "5"]) to run a "python3 main.py 5" (launch maun.py with a arg 5)
    # logName: the name of the log file
    def __init__(self, name, params, logName):
        self.name = name
        self.params = params
        self.logName = logName
        # The logging file
        self.log = None
        # Is the process running ?
        self.isRunning = False

    # Start the process
    def run(self):
        print("Starting process: " + self.name)
        self.isRunning = True
        # Open the log file
        self.log = open(self.logName, "a+")
        # Run the process, and set the standard output and error to the log file
        self.process = subprocess.Popen(self.params, stdout=self.log, stderr=self.log)

    # Get the status of the process
    # None meaning that the process is still running
    # Other value meaning that the process has ended and returning the main return code
    def checkState(self):
        return self.process.poll()

    # Stop (Kill) the process
    def stop(self):
        print("Stopping process: " + self.name)
        self.process.kill()
        # The process is not running anymore
        self.isRunning = False
        # Close the log file
        self.log.close()