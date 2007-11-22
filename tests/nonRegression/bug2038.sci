// <-- Non-regression test for bug 2038 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2038
//
// <-- Short Description -->
//    The dir command does not read properly directories different from the 
//    current
//    one.  Give the command a=dir('SubDir') (assuming SubDir exists and is 
//    non
//    empty). Then, each element of a.isdir is %f, each element of a.date and 
//    a.bytes
//    is 0.
//
//    See also
//    http://groups.google.it/group/comp.soft-
//    sys.math.scilab/browse_thread/thread/2f81e9afd1d7d6f1/e33536bff4dc1ecf#e
//    33536bff4dc1ecf

// Author : Scilab Project - Allan CORNET
// Copyright INRIA
// Date : 3 july 2006

mkdir('bug2038_dir');
cd('bug2038_dir');
mkdir('bug2038_subdir');
cd('..')

a=dir('bug2038_dir');
b=mtlb_dir('bug2038_dir');

x=getdate(a.date);
cx=x(1:4);

y=getdate();
cy=y(1:4);

T1=(a.isdir & b.isdir);
T2=(cx == cy);

if (T1 & T2) then
	affich_result(%T,2038);
else
	affich_result(%F,2038);
end
