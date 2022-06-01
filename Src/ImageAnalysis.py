import cv2
import kneed as kneed
import sklearn.cluster
from sklearn.cluster import KMeans

def transfromImage(img, mask, blurMask):
    # to grey
    greyImage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    greyMask = cv2.cvtColor(mask, cv2.COLOR_BGR2GRAY)
    blurGrey = cv2.cvtColor(blurMask, cv2.COLOR_BGR2GRAY)

    # to masked image
    maskedImage = cv2.bitwise_and(greyImage, greyImage, mask=greyMask)

    # to blurred image
    blurredImage = cv2.GaussianBlur(maskedImage, (99, 99), 0)
    intermediaryImage = maskedImage.copy()
    intermediaryImage[blurGrey==0] = blurredImage[blurGrey==0]

    finalImage = cv2.medianBlur(intermediaryImage, 7)
    finalImage = cv2.GaussianBlur(finalImage, (9, 9), 0)

    return finalImage


def drawRectangles(img, keypoints):
    minX = keypoints[0].pt[0]
    minY = keypoints[0].pt[1]
    maxX = keypoints[0].pt[0]
    maxY = keypoints[0].pt[1]
    for i in range(1, len(keypoints)):
        radius = keypoints[i].size / 2
        minX = min(minX, keypoints[i].pt[0] - radius)
        minY = min(minY, keypoints[i].pt[1] - radius)
        maxX = max(maxX, keypoints[i].pt[0] + radius)
        maxY = max(maxY, keypoints[i].pt[1] + radius)
    return cv2.rectangle(img, (round(minX) - 10, round(maxY) + 10), (round(maxX) + 10, round(minY) - 10), (0, 255, 0), 2)

def DataKMeans(keypoints):
    print("Start KMeans")
    sum_of_Squared_Distances = []
    keypointsPosition = [point.pt for point in keypoints]

    maxClusters = 10

    K = range(1, min(maxClusters, len(keypoints)))
    print("Starting knee algorithm")
    for k in K:
        print("K:", k)
        kmeans = KMeans(n_clusters=k)
        kmeans.fit(keypointsPosition)
        sum_of_Squared_Distances.append(kmeans.inertia_)
    K = kneed.KneeLocator(K, sum_of_Squared_Distances, curve='convex', direction='decreasing').knee
    print("Knee found at : ", K)
    kmeans = KMeans(n_clusters=K)
    kmeans.fit(keypointsPosition)
    clusters = []
    for i in range(K):
        clusters.append([keypoints[j] for j in range(len(keypoints)) if kmeans.labels_[j] == i])
    print("End KMeans")
    return clusters

def DBSCAN(keypoints, ep, minPts):
    print("Start DBSCAN")
    keypointsPosition = [point.pt for point in keypoints]
    db = sklearn.cluster.DBSCAN(eps=ep, min_samples=minPts).fit(keypointsPosition)
    labels = db.labels_
    clusters = []
    for i in range(max(labels) + 1):
        clusters.append([keypoints[j] for j in range(len(keypoints)) if labels[j] == i])
    print("End DBSCAN")
    return clusters


def FindCluster(img, mask, blurMask):
    transformedImage = transfromImage(img, mask, blurMask)

    # detect keypoints
    orb = cv2.ORB_create()
    keypoints, des = orb.detectAndCompute(transformedImage, None)

    hasInterestingCluster = False

    if len(keypoints) > 0:
        clusters = DBSCAN(keypoints, 100, 8)
        for cluster in clusters:
            hasInterestingCluster = True
            img = drawRectangles(img, cluster)


    return hasInterestingCluster, img