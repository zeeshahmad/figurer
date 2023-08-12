# This Python file uses the following encoding: utf-8
'''
args expressions:
    1: path to app executable containing python/texsoup directory
    2: latex content
'''

def addTexSoupToPath():
    import sys
    texSoupPath = r'%1\python\texsoup'
#    texSoupPath = r'C:\Users\zeeshan\figurer\build-figurer-project-Desktop_Qt_6_5_1_MinGW_64_bit-Debug\python\texsoup'
    if texSoupPath not in sys.path:
        sys.path.append(texSoupPath) #prepend?
addTexSoupToPath()

import TexSoup
import json


def scanDocument():
    soup = TexSoup.TexSoup(r''' %2 ''')
    includegraphics = [ig.args[-1].string for ig in soup.find_all('includegraphics')]
    result = {
        "includegraphics": includegraphics
    }
#    print(result)
    return json.dumps(result)


scan_result = scanDocument()

