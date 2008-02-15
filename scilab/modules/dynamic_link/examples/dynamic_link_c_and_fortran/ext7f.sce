
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Demo file for ext7f example 

// builder code for ext7f.c 
link_name = 'ext7f';    // functions to be added to the call table 
flag  = "f";		 // ext7f is a C function 
files = ['ext7f.o' ];   // objects files for ext7f 
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
c1=call('ext7f',a,1,'d',b,2,'d','out',2);
if norm(c1-b) > %eps then pause,end
//c now exists
if norm(c-(a+2*b)) > %eps then pause,end
//d exists 
if d<>"test" then pause,end

