// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) {date_debut_2}-{date_fin_2} - {organisme 2} - {auteur 2}
// ...
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function den=denom(r)
//returns the denominator of a rational matrix
//%Syntax: den=denom(r)
//with
//r: rational function matrix (may be polynomial or scalar matrix)
//den: polynomial matrix
//!
select type(r)
case 1 then
  den=ones(r);
case 2 then
  den=ones(r);

//-compat next case retained for list/tlist compatibility
case 15 then
  r1=r(1);
  if r1(1)<>'r' then error(92,1),end
  den=r(3)
case 16 then
  r1=r(1);
  if r1(1)<>'r' then error(92,1),end
  den=r(3)
else
  error(92,1)
end
endfunction
