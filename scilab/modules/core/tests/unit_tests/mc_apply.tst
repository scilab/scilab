// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


fun_name='test_fun_1_1';
c_prog=['#include  <math.h>'
'void '+fun_name+'(double* arg, double* res) {'
'*res= 2.*(*arg);'
'}'];
disp(c_prog);
mputl(c_prog,fun_name+'.c');
ilib_for_link(fun_name,fun_name+'.c',[],"c");
exec loader.sce;

args=[1,2];
res=mc_apply(args,fun_name,1);
if res<>[2, 4] then pause,end


fun_name='test_fun_2_1';
c_prog=['#include  <math.h>'
'void '+fun_name+'(double* arg, double* res) {'
'*res= 2.*arg[0]+ arg[1];'
'}'];
disp(c_prog);
mputl(c_prog,fun_name+'.c');
ilib_for_link(fun_name,fun_name+'.c',[],"c");
exec loader.sce;

args=[1,2;12,30];
res=mc_apply(args',fun_name,1);
if res<>[4, 54] then pause,end


fun_name='test_fun_1_2';
c_prog=['#include  <math.h>'
'void '+fun_name+'(double* arg, double* res) {'
'res[0]= *arg; res[1]= 2.* (*arg);'
'}'];
disp(c_prog);
mputl(c_prog,fun_name+'.c');
ilib_for_link(fun_name,fun_name+'.c',[],"c");
exec loader.sce;

args=[1,2];
res=mc_apply(args,fun_name,2);
if res<>[1,2;2,4] then pause,end