========================================================================
       WIN32 APPLICATION : SimpleAdpcm
========================================================================

This program is a very simple program with limited functionality, that
will let the user open a mono or stereo ADPCM wave file and direct 
the output to NVIDIA's driver, that supports ADPCM as an extensible 
format. 

There is no command line available with this application because it
is just too simple already.


/////////////////////////////////////////////////////////////////////////////
This file contains a summary of what you will find in each of the files
that make up your SimpleAdpcm application.

SimpleAdpcm.cpp
    This is the main application source file. It contains a very simple
	user interface and code to play a NVADPCM wave file.

SimpleAdpcmUI.cpp
	This file contains some functions that only have to do with the user
	interface. No hardware or nVidia specific code in this file.
	This file also contains the file validation of the extensible format.

SimpleAdpcm.dsp
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

SimpleAdpcm.h
	This is the main header file for the SimpleAdpcm application.

Common\include\dsutil.h
	This is the header file for dsutil.cpp

HwInterface.h
	This file contains all of the prototypes for HwInterface.cpp.


/////////////////////////////////////////////////////////////////////////////
Resource files:

SimpleAdpcm.rc
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
    named SimpleAdpcm.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
