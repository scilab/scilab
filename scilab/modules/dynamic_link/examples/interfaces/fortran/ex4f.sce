// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex4f.f'];
ilib_build('gw_ex4f',['ex4f','C2F(intex4f)'],files,[]);

exec loader.sce;

ex4f();
if Str<> "Pilgrim said he, where can it be this land of Eldorado" then pause;end

