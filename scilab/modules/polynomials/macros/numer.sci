// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function num=numer(r)
//returns the numerator num of a rational function matrix r (r may be
//also a scalar or polynomial matrix
//!
r1=r(1);
select type(r)
case 1 then
  num=r;
case 2 then
  num=r;

//-compat next case retained for list/tlist compatibility
case 15 then
  if r1(1)<>'r' then error(92,1),end
  num=r(2)
case 16 then
  if r1(1)<>'r' then error(92,1),end
  num=r(2)
else
  error(92,1)
end
endfunction
