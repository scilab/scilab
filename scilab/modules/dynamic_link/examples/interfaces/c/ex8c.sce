// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

files=['intex8c.c'];
ilib_build('gw_ex8c',['ex8c','intex8c'],files,[]);

exec loader.sce;

a_chain='hello';s=poly(0,'s');
deff('[y1,y2,y3]=myfunction(x1,x2)','y1=x1+x2,y2=1+s,y3=a_chain')

x1=1;x2=2;
[y1,y2,y3]=myfunction(x1,x2);

[u,v,w]=ex8c(x1,x2,myfunction);

if u-y1 > %eps then pause,end
if v-y2<>0 then pause,end
if w<>y3 then pause,end


