
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Demo file for ext10c example 

// builder code for ext10c.c 
link_name = 'ext10c';    // functions to be added to the call table 
flag  = "c";		 // ext10c is a C function 
files = ['ext10c.o' ];   // objects files for ext10c 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext10c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the ode function 
//passing a parameter to ext10c routine by a list:

param=[0.04,10000,3d+7];    
y=ode([1;0;0],0,[0.4,4],list('ext10c',param));

