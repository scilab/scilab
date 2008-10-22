// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [s]=%lss_v_s(s1,d2)
//s=%lss_v_s(s1,d2)  <=> s=s1/.d2 
//!
[a1,b1,c1,d1,x1,dom1]=s1(2:7)
e12=1/(eye()+d2*d1)
e21=eye()-d1*e12*d2;e12=b1*e12
a=a1-e12*d2*c1
s=tlist(['lss','A','B','C','D','X0','dt'],a1-e12*d2*c1,e12,e21*c1,e21*d1,x1,dom1)
endfunction
