
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

// Demo file for ext14c example

// builder code for ext14c.c
// functions to be added to the call table
link_name = ["ext14cI"];
flag  = "c";		 // ext14c is a C function
files = ["ext14c.c" ];   // objects files for ext14c
libs  = [];		 // other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext14c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//(very) simple example 1

a=[1,2,3];b=[4,5,6];n=3;
c=call("ext14cI",a,b);
if norm(c-(a+b)) > %eps then pause,end


