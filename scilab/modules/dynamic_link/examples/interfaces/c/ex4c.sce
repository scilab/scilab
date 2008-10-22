// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex4c.c'];
ilib_build('gw_ex4c',['ex4c','intex4c'],files,[]);

exec loader.sce;

//matrix (double) created by C function
x1=ex4c();
if norm(x1-matrix((1:5*3),3,5)) > %eps then pause,end
