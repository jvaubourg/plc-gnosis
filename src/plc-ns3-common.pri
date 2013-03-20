unix: {
    LD_ENV_VAR = LD_LIBRARY_PATH
}

macx: {
    LD_ENV_VAR = DYLD_LIBRARY_PATH
}

DEFINES += LD_ENV_VAR=\\\"$$LD_ENV_VAR\\\"
DEFINES += PLC_DEFAULT_NS3_PATH=\\\"$$NS3_DIR\\\"

isEmpty( NS3_DIR ){
    error( "ns3 build directory must be specified: i.e.: 'qmake NS3_DIR=/some/path/ns3.15/build etc.'")
}

isEmpty( NS3_VERSION ){
    message("NS3_VERSION Unspecified. Assuming 3.15")
    NS3_VERSION = 3.15
}

message( "Using ns3 version $$NS3_VERSION in $$NS3_DIR" )


#If the LINK_NS3 variable has been defined, setup for linking against ns3 libraries.
count(LINK_NS3, 1){
    message (" Linking vs NS3 Libraries ")

    INCLUDEPATH +=  $$NS3_DIR

    NS3_LIB = -lns$$NS3_VERSION-

    #Configuration specific settings
    CONFIG(debug, release|debug) {
        NS3_SUFFIX = -debug
    }
    else{
        NS3_SUFFIX = -debug #Future -> Release build should link against optimized ns3 libs
    }

    NS3_MODULES +=  antenna \
                    mpi \
                    applications \
                    network \
                    bridge \
                    plc \
                    config-store \
                    propagation \
                    core \
                    spectrum \
                    csma \
                    stats \
                    internet \
                    tools \
                    mobility

    for(module, NS3_MODULES) {
        NS3_LIBRARIES += $$join(module,, $$NS3_LIB, $$NS3_SUFFIX)
    }
}

