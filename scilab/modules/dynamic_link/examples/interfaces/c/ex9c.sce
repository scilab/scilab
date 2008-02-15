// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

files=['intex9c.o','as2osc.o'];
ilib_build('gw_ex9c',['ex9c','intex9c'],files,[]);

exec loader.sce;

//a's to o's
x=ex9c('gaad');
if x<>'good' then pause,end 
