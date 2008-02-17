// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [txt,vnms,vtps,nwrk]=ins2for(lst,ilst,vnms,vtps,nwrk)
// traduit un ensemble d'instructions debutant a l'adresse ilst de la
// liste courante lst
//!
nlst=size(lst)
txt=[]
while ilst<=nlst then
  if type(lst(ilst))==15 then
    [t1,vnms,vtps,nwrk]=cla2for(lst(ilst),vnms,vtps,nwrk)
    ilst=ilst+1
  else
    [t1,ilst,vnms,vtps,nwrk]=cod2for(lst,ilst,vnms,vtps,nwrk)
  end
  txt=[txt;t1]
end
endfunction
