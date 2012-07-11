// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1023 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1023
//
// <-- Short Description -->
//    mfscanf cores dump scilab. 
//
//    I have a file named links.txt, its size is 350k. When I 
//    try to run the below code, line 5 kills scilab with the 
//    error "error in fscanf : cannot allocate more memory". But 
//    the command "free" tells me I have 182Mb free memory. 
//
//    ================================================= 
//    filename = "/home/fsilva/kazus.ebooks/links.txt" 
//    inputfile = mopen(filename,"r") 
//
//    [n,a] = mfscanf(-1,inputfile,"%s"); 
//    [m,b] = mfscanf(-1,inputfile,"%s"); //line 5 
//
//    mclose(inputfile);

Tref=['xxxx';'yyy';'z'];
mputl(Tref,TMPDIR+'/bug1023.txt');
u=mopen(TMPDIR+'/bug1023.txt','r');

[n,a] = mfscanf(-1,u,"%s"); 
[m,b] = mfscanf(-1,u,"%s");

if n<>1        then pause,end
if or(a<>Tref) then pause,end
if m<>-1       then pause,end
if b<>[]       then pause,end
