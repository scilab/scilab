// Non-regression test file for bug 1957
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 15 mai 2006

mode(-1);
clear
tab=ascii(9);
path=TMPDIR;

desc=['foo'+tab+'x'+tab+'y'+tab+'[ITMAX 2000]'+tab+'[eps 0.001]'
      'x'+tab+'scalar'
      'y'+tab+'scalar'
      'ITMAX'+tab+'scalar'
      'eps '+tab+'scalar'
      'r'+tab+'vector 4'
      ''
      'foo'+tab+'x'+tab+'y'+tab+' ITMAX'+tab+'eps'+tab+'r'
      'x'+tab+'double'
      'y'+tab+'double'
      'ITMAX'+tab+'integer'
      'eps'+tab+'double'
      'r'+tab+'double'
      ''
      'out sequence'+tab+'r'
      '*******************'];
mputl(desc,path+'/intsfoo.desc')

foo_code=['#include ""stack-c.h""'
	  'void C2F(foo)(double *x,double *y,int *ITMAX,double *eps,double *r)'
	  '  {'
	  '    r[0]=*x; r[1]=*y; r[2]=*ITMAX; r[3]=*eps;'
	  '  }'];
mputl(foo_code,path+'/foo.c')

intersci=getshortpathname(SCI+'/bin/intersci-n')

curpath=pwd()
chdir(path)
try
  unix_s(intersci+' intsfoo')
  ilib_build('testlib',['test','intsfoo'],['foo.o','intsfoo.o'],[])
  exec loader.sce
catch
 affich_result(%f,1957); 
 return
end
chdir(curpath)
res=test(1.2,-2.3);
r=norm(res-[1.2,-2.3,2000,0.001])<1d-10;
res=test(1.2,-2.3,ITMAX=33);
r=r&norm(res-[1.2,-2.3,33,0.001])<1d-10;
res=test(1.2,-2.3,eps=1d-7);
r=r&norm(res-[1.2,-2.3,2000,1d-7])<1d-10;
res=test(1.2,-2.3,eps=1d-7,ITMAX=33);
r=r&norm(res-[1.2,-2.3,33,1d-7])<1d-10;
 affich_result(r,1957); 
clear
