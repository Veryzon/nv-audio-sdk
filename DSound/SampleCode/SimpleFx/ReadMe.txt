========================================================================
       WIN32 APPLICATION : SimpleFx
========================================================================

This program is a very simple program with limited functionality, that
will demonstate the ease of use in coding NVIDIA effects topologies.
The program will allow a user to open up a single wave file, then attach 
an effect to that wave file. Then the user then can play the selected 
wavefile while modifying any of the parameters associated with the 
selected effect. In this limited application the user can only select one
effect at a time, but is noted that most any type of complex topology
can be created.


/////////////////////////////////////////////////////////////////////////////
This file contains a summary of what you will find in each of the files that
make up your SimpleFx application.

SimpleFx.cpp
    This is the main application source file. It contains a very simple
	user interface and code to create the topology in the NVIDIA audio
	driver.

SimpleFxUI.cpp
	This file contains some functions that only have to do with the user
	interface. No hardware or nVidia specific code in this file.

SimpleFx.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.
	

/////////////////////////////////////////////////////////////////////////////
Common Files:

Common\source\dsutil.cpp
	This file contains all of the necessary functionality to handle all
	Direct Sound interaction. 

HwInterface.cpp
	This file contains all of the routines used to communicate with the 
	nVidia hardware. This is the only file that talks directly to the
	hardware through a Direct Sound property set.


/////////////////////////////////////////////////////////////////////////////
Header Files:

SimpleFx.h
	This is the main header file for the SimpleFx application.

SimpleOrbiter.h
	This file contains the class definition and header information only
	used by the COrbiter class.

Common\include\dsutil.h
	This is the header file for dsutil.cpp

HwInterface.h
	This file contains all of the prototypes for HwInterface.cpp.


/////////////////////////////////////////////////////////////////////////////
Resource files:

SimpleFx.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

nVidia.ico
    This is an icon file, which is used as the application's icon (32x32).


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SimpleFx.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
