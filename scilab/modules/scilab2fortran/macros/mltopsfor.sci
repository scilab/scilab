// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [res]=mltopsfor(symb)
//!
stk;
s1=stk(top-1)
s2=stk(top)
if symb+s2(2)=='**1' then s2(2)='2',end
if symb+s1(2)=='**1' then s1(2)='2',end
if symb+s2(2)=='/1' then s2(2)='2',end
if symb+s2(2)=='./1' then s2(2)='2',end
//if symb+s1(2)=='\1' then s1(2)='2',end
//if symb+s1(2)=='.\1' then s1(2)='2',end
if s2(2)=='2' then s2(1)='('+s2(1)+')',end
if s1(2)=='2' then s1(1)='('+s1(1)+')',end
res=list(s1(1)+symb+s2(1),'1')
if symb=='\' then res(2)='2',end
if symb=='.\' then res(2)='2',end
//if symb=='**' then res(2)='2',end
endfunction
