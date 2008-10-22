// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex13c.c'];
ilib_build('gw_ex13c',['ex13c','intex13c'],files,[]);

exec loader.sce;

a=ex13c([1,2,3]+%i*[4,5,5]);
if norm(a-2*[1,2,3]-%i*[4,5,5]*3) > %eps then pause,end

a=ex13c([1,2,3]);
if norm(a-2*[1,2,3]) > %eps then pause,end





