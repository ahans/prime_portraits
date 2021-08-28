import string
import random
from PIL import Image
from sklearn.cluster import kmeans_plusplus
import numpy as np
import matplotlib.pyplot as plt
import time
from prime_search import *
import sys
import pickle


img = Image.open("data/starrynight.jpg")

data = list(img.getdata())
data = np.array(data).astype(float) / 255.
centers, indices = kmeans_plusplus(data, n_clusters=10, random_state=0)

img = img.resize((img.width // 2, img.height // 2))
data = list(img.getdata())
data = np.array(data).astype(float) / 255.

print(f"working on image with resolution {img.width}x{img.height}")

data = data.reshape((img.height, img.width, 3))

start = time.time()
digits = find_prime_portrait(4, data, centers)
end = time.time()
print(f"took {end-start} s [{len(digits[0])} digits]")

with open("digits.pickle", "wb") as f:
    pickle.dump(digits, f)
with open("centers.pickle", "wb") as f:
    pickle.dump(centers, f)
