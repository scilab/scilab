
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

// Demo file for ext7f example

// builder code for ext7f.c
link_name = "ext7f";    // functions to be added to the call table
flag  = "f";		 // ext7f is a Fortran function
files = ["ext7f.f" ];   // objects files for ext7f
libs  = [];		 // other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext7f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//creating vector c in scilab internal stack


a=[1,2,3]; b=[2,3,4];
//c does not exist (c made by ext7c)
c1=call("ext7f",a,1,"d",b,2,"d","out",2);
if norm(c1-b) > %eps then pause,end
//c now exists
if norm(c-(a+2*b)) > %eps then pause,end
//d exists
if d<>"test" then pause,end

