// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [curve,ind]=datatipGetSelected(curve_handles)
//datatip utility function
//get the selected datatip(s)
  ind=[]
  for kc=1:size(curve_handles,'*')
    curve=curve_handles(kc);
    ud=datatipGetStruct(curve);
    sel=ud.selected
    if sel>0 then ind=sel,break,end
  end
  if ind==[] then curve=[],end
endfunction

