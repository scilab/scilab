
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Demo file for ext11c example

// builder code for ext11c.c
link_name = "ext11c";    // functions to be added to the call table
flag  = "c";		 // ext11c is a C function
files = ["ext11c.c" ];   // objects files for ext11c
libs  = [];		 // other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext11c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the ode function
// Passing a parameter to argument function of ode

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],"ext11c")
//param must be defined as a scilab variable upon calling ode

