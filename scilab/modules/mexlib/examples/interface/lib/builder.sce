
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
