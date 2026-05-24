========================================================================
       WIN32 APPLICATION : Simple6Chand3D
========================================================================

This program is a very simple program with limited functionality, that
will let the user open a 6 channel extensible wave file and a mono 3D
wave file. The 3D file can automatically orbit the user or the user
can manipulate the sound with the mouse. This program was written to
demonstrate that 3D and 6 Channel waves files can play simultaneously.

Also, this application can run from the command line with the following parameters.

	OrbitOn - This will automatically start the orbiter.
	StartNow - This will start all wave file playing.

	All other parameters are considered filenames.
	If a file does not exist or is an incorrect format, it will not appear in the
	applications user interface (and will show up as a blank entry).


/////////////////////////////////////////////////////////////////////////////
This file contains a summary of what you will find in each of the files
that make up your Simple6Chand3D application.

Simple6Chand3D.cpp
    This is the main application source file. It contains a very simple
	user interface and code to play a 6 channel extensible wave file along
	with a 3D mono wave file that will orbit the listener.

SimpleOrbiter.cpp
	This file contains the Orbiter class. A class that is responsible for
	calculating, drawing and maintaining an orbit in the grid window of
	the application.

Simple6Chand3DUI.cpp
	This file contains some functions that only have to do with the user
	interface. No hardware or nVidia specific code in this file.

Simple6Chand3D.dsp
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

Simple6Chand3D.h
	This is the main header file for the Simple6Chand3D application.

SimpleOrbiter.h
	This file contains the class definition and header information only
	used by the COrbiter class.

Common\include\dsutil.h
	This is the header file for dsutil.cpp

HwInterface.h
	This file contains all of the prototypes for HwInterface.cpp.


/////////////////////////////////////////////////////////////////////////////
Resource files:

Simple6Chand3D.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

nVidia.ico
    This is an icon file, which is used as the application's icon (32x32).

Grid.bmp
	This is a bitmap file, which contains the grid image that is used by the 
	Orbiter class.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Simple6Chand3D.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
