// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



fun_name='test_fun';
c_prog=['#include  <math.h>'
'void '+fun_name+'(void const* const* args, void *const* res) {'
'*((double*)*res)= 2.*(((double*)args[0])[0]);'
'}'];
disp(c_prog);
mputl(c_prog,fun_name+'.c');
ilib_for_link(fun_name,fun_name+'.c',[],"c");
exec loader.sce;
args=[1,2];

res=mc_apply(args,fun_name,1);
if res<>[2, 4] then pause,end

function a= g(arg1); a=2*arg1; endfunction; // ne marche pas avec des noms du genre f5 ...

res=mc_apply([1,2], "g")


fun_name='test_fun_2_1';
c_prog=['#include  <math.h>'
'void '+fun_name+'(double const* const* args, double ** res) {'
'res[0][0]= .5*args[0][0]; res[0][1]= .25*args[0][1];res[1][0]= 2*args[1][0];'
'}'];
disp(c_prog);
mputl(c_prog,fun_name+'.c');
ilib_for_link(fun_name,fun_name+'.c',[],"c");
exec loader.sce;

arg1=[1,2;4,5]; arg2=[2,3];
[res1, res2] =mc_apply(arg1, arg2,fun_name,"constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,6]  then pause,end

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction; // ne marche pas avec des noms du genre f5 ..
[res1, res2] =mc_apply(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,6]  then pause,end

arg1=[1,2;4,5]; arg2=[2];
[res1, res2] =mc_apply(arg1, arg2,fun_name,"constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction; // ne marche pas avec des noms du genre f5 ..
[res1, res2] =mc_apply(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end
