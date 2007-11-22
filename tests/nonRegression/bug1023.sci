// <-- Non-regression test for bug 1023 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1023
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


// Non-regression test file for bug 1023
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear
Tref=['xxxx';'yyy';'z'];
mputl(Tref,TMPDIR+'/bug1023.txt');
u=mopen(TMPDIR+'/bug1023.txt','r');

[n,a] = mfscanf(-1,u,"%s"); 
[m,b] = mfscanf(-1,u,"%s");

ok=n==1&and(a==Tref)&m==-1&b==[];
affich_result(ok,1023);

clear
