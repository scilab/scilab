
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

// Demo file for ext8c example

// builder code for ext8c.c
link_name = "ext8c";    // functions to be added to the call table
flag  = "c";		// ext8c is a C function
files = ["ext8c.c" ];   // objects files for ext8c
libs  = [];		// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext8c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[1,2,3]
y=call("ext8c","out",size(param),1,"d");
if norm(y-param) > %eps then pause,end
if norm(param(1)-18.0) > %eps then pause,end

