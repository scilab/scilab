// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

files=['intex6c.c','crestrc.c'];
ilib_build('gw_ex6c',['ex6c','intex6c'],files,[]);

exec loader.sce;

// all together 

[y1,y2,y3]=ex6c();
if y1<>"Scilab is ..." then pause,end
if norm(y2-matrix((1:5*3),3,5)) > %eps then pause,end
if norm(y3-matrix((1:5*3),3,5)) > %eps then pause,end