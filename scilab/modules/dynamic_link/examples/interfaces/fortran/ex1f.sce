// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex1f.f'];
ilib_build('gw_ex1f',['ex1f','C2F(intex1f)'],files,[]);

exec loader.sce;

A=[1,2,3,4];
B=[10,20,30;40,50,60];
//Just display A and B
ex1f(A,B)


