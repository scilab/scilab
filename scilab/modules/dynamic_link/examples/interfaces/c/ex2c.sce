// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex2c.o'];
ilib_build('gw_ex2c',['ex2c','intex2c'],files,[]);

exec loader.sce;

// Example with optional argument specified with the 
// arg=value syntax
// [a,b,c] = ex2c(x1,{ v1= arg1, v2=arg2}) , arg1 default value 99
//					       arg2 default value 3
// only v1 and v2 are recognized as optional argument names 
// the return value are a<--x1, b = 2*v2 , c = 3*v2 
//

[a,b,c]=ex2c('test');

if norm([99*2,3*3]-[b,c]) > %eps then pause,end

[a,b,c]=ex2c('test',v1=[10,20]);

if norm([[10,20]*2,3*3]-[b,c]) > %eps then pause,end

[a,b,c]=ex2c('test',v1=[10,20],v2=8);

if norm([[10,20]*2,8*3]-[b,c]) > %eps then pause,end

[a,b,c]=ex2c('test',v2=8,v1=[10]);

if norm([10*2,8*3]-[b,c]) > %eps then pause,end







