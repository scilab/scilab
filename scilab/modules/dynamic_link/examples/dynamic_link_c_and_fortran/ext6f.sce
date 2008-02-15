
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Demo file for ext6f example 

// builder code for ext6f.c 
link_name = 'ext6f';    // functions to be added to the call table 
flag  = "f";		 // ext6f is a C function 
files = ['ext6f.o' ];   // objects files for ext6f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext6f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//reading  vector with name='a' in scilab internal stack

a=[1,2,3];b=[2,3,4];
c=call('ext6f','a',1,'c',b,2,'d','out',[1,3],3,'d');
if norm(c-(a+2*b)) > %eps then pause,end





