// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



files=['intex1c.o'];
ilib_build('gw_ex1c',['ex1c','intex1c'],files,[]);

exec loader.sce;

a=1:10;b=a+1;c=ones(2,3)+2;
[x,y,z,t]=ex1c('mul',a,b,c);

 rt  = [ 2.    4.    6.    8.    10.    12.    14.    16.    18.    20.];
 rz  = [ 4.    6.    8.    10.    12.    14.    16.    18.    20.    22.];
 ry  = [ 6.    6.    6.;
        6.    6.    6.];
 rx  = [ 0.    6.     12.;
        6.    12.    18.];
        
if rt <> t then pause,end
if rz <> z then pause,end
if rx <> x then pause,end
if ry <> y then pause,end
