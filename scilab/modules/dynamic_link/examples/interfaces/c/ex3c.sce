// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

files=['intex3c.c'];
ilib_build('gw_ex3c',['ex3c','intex3c'],files,[]);

exec loader.sce;


A = rand(10,10,'n')>=0 ;

B=ex3c(A);

