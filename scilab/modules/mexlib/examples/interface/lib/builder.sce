
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Demo file for ext1c example

// builder code for ext1c.c
link_name = "util" ;    	// functions to be added to the call table
// the first entry point gives the name of the library
// note here that util is not an entry point
// thus the generated loader.sce will not work
// but it does not matter since we will use an other
// loader.sce (../lib/loader.sce)

flag  = "c";			// ext1c is a C function
files = ["foo.o";"bar.o" ];   	// objects files for ext1c
libs  = [];			// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext1c and performs the compilation

ilib_for_link(link_name,files,libs,flag);
