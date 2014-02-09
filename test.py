#! /usr/bin/python
from recog import Recognizer
import os

lmPath = os.getcwd()
r = Recognizer(lmPath)
print r.recog_word()