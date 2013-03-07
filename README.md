#PLC-Gnosis (Working Title)

>*This document is a work in progress as the build process is still being finalized pending successful (and painless) compilation on our target platforms, as well as the consolidation of all the instructions and prerequisites. It is subject to change and we suggest that you procure a heafty supply of salt to take while reading it.* 

This software is the GUI for an experimental power line communication module built around the ns-3 network simulator.

##Overview

The purpose of this software is to abstract away the difficult and time-consuming process of hand-piecing together power line communication network models from scratch in c++ using the ns-3 simulator. 

The GUI provides most of the tools necessary to graphically represent the available network components and provides an intuitive interface for adjusting model parameters, cable types, noise sources and others. Also built by this package is a library through which models created in the GUI can be loaded into hand-crafted simulation code. This allows for extreme flexibility in both method and purpose when using the ns-3 plc module to model power line communication networks.

##Build Instructions

A lot of work has been put into making sure that this code is easy to build and use. Should anyone have any problems please post in the issue tracker and we will attempt to fix it as soon as possible. 

Building software successfully should never be a headache... even though it often is.

If you want to skip to less verbose instructions, please see the tl;dr version below.

###Prerequisites

The first and most obvious prerequisite is downloading the source code. If you're reading this document then you have probably already found it. If not however, please wander over to our [GitHub](http://github.com) page [here](http://github.com/Valiance/plc-gnosis "PLC-Gnosis Main Git Repository"). Clone the repository into a directory of your choosing.

###Qt

The GUI has been designed using the Qt GUI Framework and SHOULD work with the all 4.8 series of releases. We have TESTED mostly with 4.8.3, so please let us know if you run into any problems with your specific version. We will port to Qt 5 when it becomes a little more widespread.

Please see 

###*[QJson](https://github.com/flavio/qjson "GitHub page for QJson")*

The QJson library provides us with an elegant way of interfacing our Qt based GUI with our JSON based format for diagram files. Shared libraries are often a massive pain and since we would like to keep our instructions as platform-agnostic as possible, we compile QJson directly and link statically. Satisfying this prerequisite involves making a clone of the QJson repository into the lib directory of our source tree as follows:

	cd plc-gnosis/lib
	git clone git://github.com/flavio/qjson.git qjson

You are done with this prerequisite once the cloning completes.
