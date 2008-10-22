// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [nwrk]=dclfun(nwrk,nam,typ)
//permet de declarer le type fortran du resultat d'une fonction
//!
select typ
case '0' then
  if find(nam==nwrk(14))==[] then  nwrk(14)=[nwrk(14);nam],end
case '1' then
  if find(nam==nwrk(15))==[] then  nwrk(15)=[nwrk(15);nam],end
end
endfunction
