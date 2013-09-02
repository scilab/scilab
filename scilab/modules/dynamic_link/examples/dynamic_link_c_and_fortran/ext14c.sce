
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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


