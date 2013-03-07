#Contains all the information regarding the location of the NS3 libraries and Qwt
#Future versions will ditch the Qwt dependency however.

########MODIFY THESE VARIABLES TO POINT TO THE APPROPRIATE PATHS ON YOUR SYSTEM########
#QWT_LIB_DIR = $$quote(/usr/local/qwt-6.0.1/lib)
#QWT_INC_DIR = $$quote(/usr/local/qwt-6.0.1/include)

isEmpty( NS3_DIR ){
    error( "ns3 build directory must be specified: i.e.: 'qmake ns-3gui.unix.pro NS3_DIR =/some/path/ns3.15/build")
}

isEmpty( NS3_VERSION ){
    message("NS3_VERSION Unspecified. Assuming 3.15")
    NS3_VERSION = 3.15
}

message( "Using ns3 version $$NS3_VERSION in $$NS3_DIR" )
#######################################################################################

NS3_LIB = -lns$$NS3_VERSION


#Configuration specific settings
CONFIG(debug, release|debug) {
    NS3_SUFFIX = -debug
}
else{
    NS3_SUFFIX = -debug #Future -> Release build should link against optimized ns3 libs
}

NS3_LIBRARIES = $$NS3_LIB-antenna$$NS3_SUFFIX \
                $$NS3_LIB-mpi$$NS3_SUFFIX \
                $$NS3_LIB-applications$$NS3_SUFFIX \
                $$NS3_LIB-network$$NS3_SUFFIX \
                $$NS3_LIB-bridge$$NS3_SUFFIX \
                $$NS3_LIB-plc$$NS3_SUFFIX \
                $$NS3_LIB-config-store$$NS3_SUFFIX \
                $$NS3_LIB-propagation$$NS3_SUFFIX \
                $$NS3_LIB-core$$NS3_SUFFIX \
                $$NS3_LIB-spectrum$$NS3_SUFFIX \
                $$NS3_LIB-csma$$NS3_SUFFIX \
                $$NS3_LIB-stats$$NS3_SUFFIX \
                $$NS3_LIB-internet$$NS3_SUFFIX \
                $$NS3_LIB-tools$$NS3_SUFFIX \
                $$NS3_LIB-mobility$$NS3_SUFFIX

DEFINES += PLC_DEFAULT_NS3_PATH=\\\"$$NS3_DIR\\\"

#INCLUDEPATH += $$QWT_INC_DIR

