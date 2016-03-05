

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
// Demo file for ext1f example

// builder code for ext1f.c
link_name = "ext1f";    // functions to be added to the call table
flag  = "f";		// ext1f is a Fortran function
files = ["ext1f.f" ];   // objects files for ext1f
libs  = [];		// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext1f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//(very) simple example 1

a=[1,2,3];b=[4,5,6];n=3;
c=call("ext1f",n,1,"i",a,2,"d",b,3,"d","out",[1,3],4,"d");
if norm(c-(a+b)) > %eps then pause,end

