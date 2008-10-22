// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [r]=%lss_n_s(s1,s2)
//%lss_n_s(s1,s2) test s1<>s2
r=s1(2)<>[]|s1(5)<>s2
endfunction
