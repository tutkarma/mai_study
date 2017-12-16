# -*- coding: utf-8 -*-
import re

def findID(line):
    ID = re.search(r"^0\s+@I(\d+)@\s+INDI$", line)
    if ID is not None:
        return ID.group(1)

def findName(line):
    Name = re.search(r"^1\s+NAME\s+(.+)\s+/(.+)/$", line)
    if Name is not None:
        return Name.group(1) + " " + Name.group(2)

def findSex(line):
    Sex = re.search(r"^1\s+SEX\s+([F|M])$", line)
    if Sex is not None:
        return Sex.group(1)
        
if __name__ == '__main__':
    outFile = open("res", "w")

    fileLines = []
    with open("dovzhenko") as inFile:
        fileLines = [row.strip() for row in inFile]

    data = {}
    idx = list(filter(None, map(findID, fileLines)))
    names = list(filter(None, map(findName, fileLines)))
    data = {ID: Name for ID, Name in zip(idx, names)}
    sex = list(filter(None, map(findSex, fileLines)))
    for s, key in zip(sex, data):
        if s == "M":
            outFile.write("male(\'{0}\').\n".format(data[key]))
        elif s == "F":
            outFile.write("female(\'{0}\').\n".format(data[key]))

    # по хорошему, цикл нужно убрать
    childPred = []
    it = iter(fileLines)
    while True:
        try:
            line = next(it)
            findFather = re.search(r"1\s+HUSB\s+@I(\d+)@", line)
            if findFather is not None:
                father = data[findFather.group(1)]
                continue

            findMother = re.search(r"1\s+WIFE\s+@I(\d+)@", line)
            if findMother is not None:
                mother = data[findMother.group(1)]
                continue

            findChild = re.search(r"1\s+CHIL\s+@I(\d+)@", line)
            if findChild is not None:
                buf = (data[findChild.group(1)], mother)
                childPred.append(buf)
                buf = (data[findChild.group(1)], father)
                childPred.append(buf)
                continue
        except StopIteration:
            break
    
    printPred = lambda x, y: outFile.write("child(\'{0}\', \'{1}\').\n".format(x, y))
    for count, i in enumerate(childPred):
        child, parent = childPred[count]
        printPred(child, parent)
    outFile.close()