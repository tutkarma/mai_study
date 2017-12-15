# -*- coding: utf-8 -*-
import re

inFile = open("dovzhenko", "r")
outFile = open("res", "w")

data = {}
male = []
female = []

idx = ""
name = ""
father = ""
mother = ""

for line in inFile:

    findID = re.search(r"^0\s+@I(\d+)@\s+INDI$", line)
    if findID is not None:
        idx = findID.group(1)
        continue

    findName = re.search(r"^1\s+NAME\s+(.+)\s+/(.+)/$", line)
    if findName is  not None:
        name = findName.group(1) + " " + findName.group(2)
        data[idx] = name
        continue

    findSex = re.search(r"^1\s+SEX\s+([F|M])$", line)
    if findSex is not None:
        if findSex.group(1) == "F":
            female.append(name)
        elif findSex.group(1) == "M":
            male.append(name)
        continue

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
        outFile.write("child(\'{0}\', \'{1}\').\n".format(data[findChild.group(1)], mother))
        outFile.write("child(\'{0}\', \'{1}\').\n".format(data[findChild.group(1)], father))
        continue


for i in range(len(male)):
    outFile.write("male(\'{0}\').\n".format(male[i]))
for i in range(len(female)):
    outFile.write("female(\'{0}\').\n".format(female[i]))


inFile.close()
outFile.close()