// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;
BugNumber=2420;
function a=strudel(),a='+\@+';endfunction
t=fun2string(strudel)    
T=stripblanks(t(2))=="a = ''+\@+'';"
affich_result(T,BugNumber) ;
clear ;
