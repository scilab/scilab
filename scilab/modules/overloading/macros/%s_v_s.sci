// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [h]=%s_v_s(h1,h2)
    // %s_v_s(h1,h2)  <=> h =(I+h1*h2)\h1 <=> h=h1/.h2

    [m1,n1]=size(h1)
    [m2,n2]=size(h2)
    if abs(n1-m2)+abs(m1-n2)<>0 then error(msprintf(_("%s: Wrong size for input argument(s).\n"),"%s_v_s")),end
    h=(eye(m1,m1)+h1*h2)\h1
endfunction
