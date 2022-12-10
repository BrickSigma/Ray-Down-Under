import os
from PIL import Image
import binascii

os.chdir("levels/level 4")
files = os.listdir()

files = [i for i in files if ("layer" in i and "png" in i)]

objects = {
        "\0" : (255, 255, 255),  # Empty space
        "\1" : (0, 0, 0),  # Base tile
        "\2" : (255, 0, 0),  # Lever
        "\3" : (0, 0, 255),  # Spawn
        }

with open("../../resources/level 4.txt", "wb") as f:
    img = Image.open(files[0])
    pix = img.load()
    w, h = img.size
    f.write(bytes(f"{w} {h} {len(files)}\n", encoding='utf-8'))
    print(f"{w} {h} {len(files)}")
    img.close()

    for i in files:
        img = Image.open(i)
        pix = img.load()
        string = ""

        for h in range(img.size[1]):
            for w in range(img.size[0]):
                for key, _ in objects.items():
                    if (pix[w, h][0:3] == objects[key]):
                        string += key
            string += "\n"
        
        f.write(bytes(string, encoding='utf-8'))

    img.close()