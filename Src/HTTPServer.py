import http.server
import sqlite3
import socketserver
import time

from Util import ServerMessage

####### This file contains the code for the configuration server.

# The default output directory
DIRECTORY = ["E:\\"]

# A class that wrap the http server
class DataHTTPServer():
    # Initialize the server with the root directory, the port and the communication queue
    def __init__(self, directory, port, input_queue, output_queue):
        # Setting the root directory
        DIRECTORY[0] = directory
        self.port = port
        self.input_queue = input_queue
        self.output_queue = output_queue

    # Start the server
    def start(self):
        # Creating the request handler and setting the communication queues
        self.handler = Handler
        self.handler.input = self.input_queue
        self.handler.output = self.output_queue
        # Starting the server
        with socketserver.TCPServer(("", self.port), self.handler) as httpd:
            httpd.serve_forever()


# The request handler
class Handler(http.server.SimpleHTTPRequestHandler):
    # The communication queues
    input = None
    output = None

    def __init__(self, *args, **kwargs):
        # Call the parent constructor and specify the root directory
        super().__init__(*args, directory=DIRECTORY[0], **kwargs)

    # Handle the GET request
    def do_GET(self):
        # If the client request the URL for the Board, we ask the main thread to generate the board
        # See StarFishScanner.py and generateControlBoard() from Util.py for more detail
        if self.path == "/configuration/":
            self.sendToMain(ServerMessage.GENERATE_FORM)
        # Then we call the parent method to handle the request that return the requested file
        super().do_GET()

    # Method that send a message to the main thread and wait for the response (Synchronous communication)
    def sendToMain(self, code):
        # We send the message to the main thread
        self.output.put(code)
        # We wait for the response
        while self.input.empty():
            time.sleep(0.1)
        # We empty the input queue and continue serving
        self.input.get()

    # Handle the POST request
    def do_POST(self):
        # If the client request for capture to start
        if self.path == "/changeParameters":
            # get range input
            header = self.rfile.read(int(self.headers['Content-Length'])).decode('utf-8')
            paramsDict = {key: value for key, value in [arg.split('=') for arg in header.split("&")] if key != "submit"}

            sqlConnection = sqlite3.connect("database.db")
            sqlConnection.execute("UPDATE SFParameters SET value = ? WHERE name = ?", (int(paramsDict["range"]), "range"))
            sqlConnection.execute("UPDATE SFParameters SET value = ? WHERE name = ?", (int(paramsDict["samples"]), "samples"))
            sqlConnection.execute("UPDATE SFParameters SET value = ? WHERE name = ?", (int(paramsDict["vos"]), "vos"))
            sqlConnection.execute("UPDATE VisualisationParameters SET value = ? WHERE name = ?", (int(paramsDict["gain"]), "gain"))
            sqlConnection.execute("UPDATE VisualisationParameters SET value = ? WHERE name = ?", (int(paramsDict["contrast"]), "contrast"))
            sqlConnection.execute("UPDATE MiscParameters SET value = ? WHERE name = ?", (int(paramsDict["caplen"]), "timeseparation"))
            sqlConnection.execute("UPDATE MiscParameters SET value = ? WHERE name = ?", (paramsDict["cmap"], "cmap"))
            sqlConnection.commit()

            self.sendToMain(ServerMessage.PARAMETERS_CHANGED)

            self.path = "/"
        # Then we serve as a GET request
        print("RETURN GET")
        super().do_GET()

# A class that wrap the http server
class GrafanaDataHTTPServer():
    # Initialize the server with the root directory, the port and the communication queue
    def __init__(self, directory, port, input_queue, output_queue):
        # Setting the root directory
        DIRECTORY[0] = directory
        self.port = port
        self.input_queue = input_queue
        self.output_queue = output_queue

    # Start the server
    def start(self):
        # Creating the request handler and setting the communication queues
        self.handler = GrafanaHandler
        self.handler.input = self.input_queue
        self.handler.output = self.output_queue
        # Starting the server
        with socketserver.TCPServer(("", self.port), self.handler) as httpd:
            httpd.serve_forever()


# The request handler
class GrafanaHandler(http.server.SimpleHTTPRequestHandler):
    # The communication queues
    input = None
    output = None

    def __init__(self, *args, **kwargs):
        # Call the parent constructor and specify the root directory
        super().__init__(*args, directory=DIRECTORY[0], **kwargs)

    def sendToMain(self, code):
        # We send the message to the main thread
        self.output.put(code)
        # We wait for the response
        while self.input.empty():
            time.sleep(0.1)
        # We empty the input queue and continue serving
        self.input.get()

    def do_OPTIONS(self):
        if self.path == "/switch":
            self.send_response(200)
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            # We send the message to the main thread indication to switch the capture
            self.sendToMain(ServerMessage.SWITCH_MODE)