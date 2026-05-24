========================================================================
       WIN32 APPLICATION : SimpleSubmix
========================================================================

This program is a very simple program with limited functionality, that
will demonstate the powerful usage of the NVIDIA Submix technology.
The program will allow the user to open up 1 to 6 different wave files
and attach these wave files to a single submix. Then this submix can be
moved around in the 3D space, automatically or manually (with the mouse).
This is very powerful in that the programmer does not have to maintain
positional information for all the sound sources, only the submix
itself. Wave files can be attached to the submix before the submix 
is played, or while the submix is playing.

Also, this application can run from the command line with the following parameters.

	OrbitOn - This will automatically start the orbiter.
	StartNow - This will start all wave file playing.
	Always - If this appears after a wavefile, then that sound will loop forever.
	Random - If this appears after a wavefile, then that sound will not be
			 looping an will restart randomly withing 0 to 10 seconds after
			 it terminates.
	All other parameters are considered filenames.
	If a file does not exist or is an incorrect format, it will not appear in the
	applications user interface (and will show up as a blank entry).


/////////////////////////////////////////////////////////////////////////////
This file contains a summary of what you will find in each of the files
that make up your SimpleSubmix application.

SimpleSubmix.cpp
    This is the main application source file. It contains a very simple
	user interface and code to create the topology in the NVIDIA audio
	driver.

SimpleOrbiter.cpp
	This file contains the Orbiter class. A class that is responsible for
	calculating, drawing and maintaining an orbit in the grid window of
	the application.

SimpleSubmixUI.cpp
	This file contains some functions that only have to do with the user
	interface. No hardware or nVidia specific code in this file.

SimpleSubmix.dsp
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

SimpleSubmix.h
	This is the main header file for the SimpleSubmix application.

SimpleOrbiter.h
	This file contains the class definition and header information only
	used by the COrbiter class.

Common\include\dsutil.h
	This is the header file for dsutil.cpp

HwInterface.h
	This file contains all of the prototypes for HwInterface.cpp.


/////////////////////////////////////////////////////////////////////////////
Resource files:

SimpleSubmix.rc
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
    named SimpleSubmix.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
