// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1957 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1957
//
// <-- Short Description -->
//    Non fonctionnement de intersci pour les variables optionnelles (au 
//    moins scalaires)

tab  = ascii(9);
path = TMPDIR;

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

intersci=getshortpathname(SCI+'/modules/intersci/bin/intersci-n')

curpath=pwd();
chdir(path);

try
	unix_s(intersci+' intsfoo')
	ilib_build('testlib',['test','intsfoo'],['foo.o','intsfoo.o'],[])
	exec loader.sce
catch
	if %T then pause,end
	return
end

chdir(curpath);

res=test(1.2,-2.3);
if norm(res-[1.2,-2.3,2000,0.001])  >= 1d-10 then pause,end

res=test(1.2,-2.3,ITMAX=33);
if norm(res-[1.2,-2.3,33,0.001])    >= 1d-10 then pause,end

res=test(1.2,-2.3,eps=1d-7);
if norm(res-[1.2,-2.3,2000,1d-7])   >= 1d-10 then pause,end

res=test(1.2,-2.3,eps=1d-7,ITMAX=33);
if norm(res-[1.2,-2.3,33,1d-7])     >= 1d-10 then pause,end
