# This Python file is for latexfileparser cpp class, whose tests also test this
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
    result = {}
    try:
        soup = TexSoup.TexSoup(r''' %2 ''')
        includegraphics = [str(ig.args[-1].string) for ig in soup.find_all('includegraphics')]
        includegraphics = list(set(includegraphics)) #remove / ignore duplicates
        result = {
            "includegraphics": includegraphics
        }
    except EOFError as e:
        result = {
            "error": f"EOFError: {e}"
        }
    return json.dumps(result)


scan_result = scanDocument()

