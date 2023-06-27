import os
import sqlite3
import sys
import time
from datetime import datetime
from os import listdir
from os.path import isfile, join
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
from matplotlib import cm

from ImageAnalysis import FindCluster

#### This file is used to generate the images from the CSV capture files

# Contains the list of the processed files
TREATED_FILES = []

# Colors for the depth graphics
DIRT_COLOR = (124 / 256, 94 / 256, 66 / 256)
WATER_COLOR = (89 / 256, 129 / 256, 167 / 256)
# The normalized color palette for the graphic representation (Gain and contrast added)
PALETTE = [(i) for i in range(256)]
# The base color palette
COLOR_PALETTE = []
# Screen DPI
SCREEN_DPI = 96
# Parameters for detecting the seabed
# The needed consecutive values above DEPTH_THRESHOLD to be considered as a seabed
DEPTH_STREAK_THRESHOLD = 15
# The minimum echo value to be considered as potential seabed (in Db)
DEPTH_THRESHOLD = 28


# This function fetch the color palette from matplotlib and convert it to a list of RGB values
def recolorPalette(cmap):
    COLOR_PALETTE.extend([(round(x[0] * 255), round(x[1] * 255), round(x[2] * 255)) for x in
                          cm.get_cmap(cmap)([i for i in range(256)])[np.newaxis, :, :3].tolist()[0]])


# This function read all the data from the given file and return the lines as an array
def readData(file):
    with open(file, "r") as f:
        data = f.readlines()
        return data

# This function generate the normalized color palette
def calculatePalette(gain, contrast):
    # For each possible signal we find the color to map
    for i in range(256):
        # We normalize the signal by substracting the gain (Gain is negative in CSV) and dividing by the contrast
        normalizedSignal = ((i / 2) + gain) / contrast
        # We clip the signal to the 0-1 range (See StarFishSDK documentation page 19 for more informations)
        normalizedSignal = min(normalizedSignal, 1)
        normalizedSignal = max(normalizedSignal, 0)
        # We find the corresponding index in the color palette for that signal
        idx = round(normalizedSignal * 255)
        # We add the color to the palette
        PALETTE[i] = COLOR_PALETTE[idx]

# This function extract the gain and contrast from the CSV file
# WARNING : Because the function fetch gain and contrast from the first line
# WARNING : if gain and contrast values are modified, the result may be wrong
def extractVisualisationData(data):
    # We fetch the gain and contrast from the first line (See documentation for CSV format)
    try:
        samples = int(data[0].split(",")[4])
        contrast = float(data[0].split(",")[1])
        gain = float(data[0].split(",")[2])
        return (samples, contrast, gain)
    except:        
        return (-1, -1, -1)


# This function save the given image to the given path
def saveImage(img, path):
    img.save(path)

# This function generate the graphic representation of the depth below the sonar
# depth : List containing all the depth values
# return the graph as a PIL image
def drawDepth(depths):
    # The width of the image is the number of values in the depth list
    baseYSize = len(depths)
    # We create a new graphic with matplotlib and specifying the size to be one pixel per depth value
    fig = plt.figure(figsize=((baseYSize) / SCREEN_DPI, 5), dpi=SCREEN_DPI * 1.29)
    ax = fig.add_subplot(111)
    # We plot the depth values
    ax.plot(depths)
    # We remove the x axis (we don't need it)
    ax.get_xaxis().set_visible(False)
    # We set the range of Y value to  be between 0 and the maximum depth value
    plt.ylim([0, max(depths)])
    # We invert the axis (We want the surface to be on the top)
    ax.invert_yaxis()
    # We remove the margin on the left and right
    plt.margins(x=0)
    # We colorize the graph
    # The area below the graph is dirt
    ax.fill_between([i for i in range(baseYSize)], max(depths), depths, color=DIRT_COLOR, alpha=0.5)
    # The area above the graph is water
    ax.fill_between([i for i in range(baseYSize)], depths, 0, color=WATER_COLOR, alpha=0.8)
    # We make the y ticks to be inside the graph (otherwise they are out of the final image)
    ax.tick_params(axis='y', direction='in', pad=-22)
    # We save the graph as a PNG image
    fig.savefig("depth.png", dpi="figure", bbox_inches='tight', pad_inches=0)
    # We return the saved image
    return Image.open("depth.png")

# This function allows us to merge two images
# Caution : The first image is rotated by 90°
def mergeImages(img1, img2):
    dataImage = Image.fromarray(img1)
    # We calculate the total width (Because we flip the final image 90°, the second image width is his height)
    totalWidth = dataImage.width
    # The final height is the height of the fist image (Because we flip the final image 90°)
    totalHeight = img1.shape[0] + img2.height
    # We create a new RGB image
    merged = Image.new("RGB", (totalWidth, totalHeight), color="Black")
    # We paste the first image on the top left
    merged.paste(dataImage, (0, 0))
    merged.paste(img2, (0, img1.shape[0]))
    # We return the merged image with a 270° rotation
    return merged

# This function draw the CSV data on a image
def drawData(data):
    # We extract the contrast and gain and generate the color palette
    samples, contrast, gain = extractVisualisationData(data)

    if(samples and contrast and gain == -1):
        return (-1, -1)

    contrast = max(contrast, 1)
    calculatePalette(gain, contrast)

    y = len(data) - 1

    image = np.zeros([samples*2, y, 3], dtype=np.uint8)
    mask = np.zeros(image.shape, dtype=np.uint8)
    blurMask = np.zeros(image.shape, dtype=np.uint8)

    depths = []

    # We iterate over the data (Represent one pixel line)
    for line in range(y):
        # We extract the comma separated values
        values = data[line].split(",")

        ldepth = -1
        lstreak = 0
        lsamples = int(values[4])
        # For details on the CSV format, see the documentation
        lrange = float(values[3])
        ldata = values[5:5 + lsamples]

        # We calculate the meter per pixel ratio
        lscale = lrange / lsamples

        # For each Port samples
        for x in range(lsamples):
            if ldepth != -1:
                mask[lsamples - x, line] = 255
                try:
                    blurMask[lsamples - x - 20, line] = 255
                except:
                    pass
            # We draw the pixel to his position with color from the previous calculated palette
            image[lsamples - x, line] = PALETTE[int(ldata[x])]
            # If the pixel is not too close to the center (Surface parasite echo)
            if x > 10:
                # We normalize the echo value
                xEchoValue = (int(ldata[x]) - (DEPTH_THRESHOLD * 2))
                # If the current value is considered as potential seabed and we haven't found seabed yet
                if xEchoValue > 0 and ldepth == -1:
                    # We grow the streak
                    lstreak += 1
                    # If the streak is long enough
                    if lstreak > DEPTH_STREAK_THRESHOLD:
                        # We draw a red pixel to the found seabed position
                        image[lsamples - x + DEPTH_STREAK_THRESHOLD, line] = (255, 0, 0)
                        # We save the seabed depth
                        ldepth = (x - DEPTH_STREAK_THRESHOLD) * lscale
                # If the pixel is not considered as potential seabed (Potential false seabed)
                elif xEchoValue <= 0:
                    # We reset the streak
                    lstreak = 0

        ### Same as before but for the Starboard side
        rdepth = -1
        rstreak = 0

        rrange = float(values[8 + lsamples])
        rdata = values[10 + lsamples:]

        rscale = rrange / lsamples

        for x in range(lsamples):
            if rdepth != -1:
                mask[lsamples + x, line] = 255
                try:
                    blurMask[lsamples + x + 20, line] = 255
                except:
                    pass
            # Caution : Because of the mirroring, the position of the pixel is inverted
            image[x + lsamples, line] = PALETTE[int(rdata[x])]
            if x > 10:
                xEchoValue = (int(rdata[x]) - (DEPTH_THRESHOLD * 2))
                if xEchoValue > 0 and rdepth == -1:
                    rstreak += 1
                    if rstreak > DEPTH_STREAK_THRESHOLD:
                        image[x + lsamples - DEPTH_STREAK_THRESHOLD, line] = (255, 0, 0)
                        rdepth = (x - DEPTH_STREAK_THRESHOLD) * rscale
                elif xEchoValue <= 0:
                    rstreak = 0

        # We save the mean of the depth found on the two sides
        depths.append(round((ldepth + rdepth) / 2, 2))

        y -= 1

    hasInterestingCluster, testIm = FindCluster(image, mask, blurMask)
    # We generate the depth image
    depthImg = drawDepth(depths)

    # We return the combined image
    return hasInterestingCluster, mergeImages(testIm, depthImg)


if __name__ == '__main__':
    # DEBUG MODE
    if len(sys.argv) < 4:
        LOG_PATH = "dataLogs"
        IMAGE_PATH = "images"
        CMAP = "copper"
    # PRODUCTION MODE
    else:
        LOG_PATH = sys.argv[1]
        IMAGE_PATH = sys.argv[2]
        INTEREST_IMAGE_PATH = join(IMAGE_PATH, "interestClusters")
        NON_INTEREST_IMAGE_PATH = join(IMAGE_PATH, "nonInterestClusters")
        CMAP = sys.argv[3]
        # All the files already present in the folder will not be processed
        TREATED_FILES = [f for f in listdir(LOG_PATH) if isfile(join(LOG_PATH, f))]
    # We fetch the color palette
    recolorPalette(CMAP)

    print("\n\n\n********************************************************************************")
    print("LOG TIMESTAMP: " + str(datetime.now()))
    print("********************************************************************************\n")
    print("Generating images...")

    sqliteConnection = sqlite3.connect("database.db")

    while True:
        # For each CSV file that are not already treated
        for f in [f for f in listdir(LOG_PATH) if isfile(join(LOG_PATH, f)) if f not in TREATED_FILES]:
            # Security check, we don't want to start processing a file that have not been saved yet
            while os.path.getsize(join(LOG_PATH, f)) == 0:
                time.sleep(0.1)
            # Remember that the file has been treated
            TREATED_FILES.append(f)
            print("Generating images for " + f)
            # Process the file
            data = readData(join(LOG_PATH, f))

            hasInterstingCluster, img = drawData(data)
            if(hasInterstingCluster == -1):
                print(f"Error while generating, skipping !\n")
            else:
                if hasInterstingCluster:
                    saveImage(img, join(INTEREST_IMAGE_PATH, f.replace(".csv", ".png").lstrip('treated')))

                    sqliteConnection.execute(f"UPDATE RuntimeVariable SET value = '/images/interestClusters/{f.replace('.csv', '.png').replace('record', 'cord').lstrip('treated')}' WHERE name = 'lastImagePath'")
                else:
                    saveImage(img, join(NON_INTEREST_IMAGE_PATH, f.replace(".csv", ".png").lstrip('treated')))
                    sqliteConnection.execute(f"UPDATE RuntimeVariable SET value = '/images/nonInterestClusters/{f.replace('.csv', '.png').replace('record', 'cord').lstrip('treated')}' WHERE name = 'lastImagePath'")
                sqliteConnection.commit()

                print(f"{f.replace('.csv', '.png').lstrip('treated')} generated !\n")
        # We sleep to avoid consuming too much CPU and battery
        time.sleep(1)
    print("Done")
