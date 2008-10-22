// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex3f.f'];
ilib_build('gw_ex3f',['ex3f','C2F(intex3f)'],files,[]);

exec loader.sce;

// Accessing a Scilab string inside an interface 
Mystr='My string';
ex3f()

