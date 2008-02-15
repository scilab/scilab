// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

files=['intex11c.o'];
ilib_build('gw_ex11c',['ex11c','intex11c'],files,[]);

exec loader.sce;

deff('[u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
//ex11c executes myfct with input variables x=5 and y=6
x=5;y=6;[u,v,w]=myfct(x,y);
[p,q,r]=ex11c(1,2,3,4,5,6);
if p<>u | q <> v | r <> w then pause,end

