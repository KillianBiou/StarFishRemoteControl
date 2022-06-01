from os import listdir
from os.path import isfile, join

def normalizeStrarlinelogs(data):
    normalizedData = []
    for line in data:
        content = line.split(",")
        if(content[0] == "SSS"):

            side = content[2]
            contrast = content[3]
            gain = content[5]
            range = content[6]
            samples = content[7]
            data = content[8:8+int(samples)]

            targetDataP = [side, contrast, gain, range, samples]
            targetDataP.extend(data)


            starboardStartIndex = round(len(content) / 2) + 1
            side = content[starboardStartIndex]
            contrast = content[starboardStartIndex + 1]
            gain = content[starboardStartIndex + 3]
            range = content[starboardStartIndex + 4]
            samples = content[starboardStartIndex + 5]
            data = content[starboardStartIndex+6:]

            targetDataS = [side, contrast, gain, range, samples]
            targetDataS.extend(data)

            targetData = targetDataP
            targetData.extend(targetDataS)

            normalizedData.append(targetData)
    return normalizedData


def treatFile(name):
    with open(name, "r") as f:
        lines = f.readlines()
        writeName = "logs\\treated" + name.split('\\')[-1]
        print(writeName)
        with open(writeName, "w+") as fw:
            for data in normalizeStrarlinelogs(lines):
                fw.write(",".join(data))

if __name__ == '__main__':
    PATH = "untreated"

    print("Normalizing StarLine logs")
    for log in [f for f in listdir(PATH) if isfile(join(PATH, f))]:
        print(log)
        treatFile(join(PATH, log))
        print(f"{log} treated ...")
    print("Done")
