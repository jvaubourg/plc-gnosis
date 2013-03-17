#Setup required Qt modules
QT += gui svg opengl

#Build template
TEMPLATE = subdirs

BIN_DIRECTORY = $$PWD/../bin

SUBDIRS +=  PLCTopologyLoader \
            PLCGui \
            PLCSimulator \
            Launcher

CONFIG += ordered
