// Non-regression test file for bug 1023
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear
Tref=["xxxx";"yyy";"z"];
mputl(Tref,TMPDIR+"/bug1023.txt");
u=mopen(TMPDIR+"/bug1023.txt","r");

[n,a] = mfscanf(-1,u,"%s");
[m,b] = mfscanf(-1,u,"%s");

ok=n==1&and(a==Tref)&m==-1&b==[];
affich_result(ok,1023);

clear
