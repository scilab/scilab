// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex5c.o','crestrc.o'];
ilib_build('gw_ex5c',['ex5c','intex5c'],files,[]);

exec loader.sce;

//Character string created by C function
x3=ex5c();
if x3<>"Scilab is ..." then pause,end