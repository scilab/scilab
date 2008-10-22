// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [txt]=makeargs(args)
//etant donne un vecteur de chaines de caracteres "args", makeargs(args)
//retourne une chaine de la forme arg(1)+,+....+,+arg(n)
//!
n=prod(size(args))
txt=' '
if n==0 then return,end
for k=1:n,
  if args(k)=='eye' then
    txt=txt+':,'
  else
    txt=txt+args(k)+','
  end,
end
txt=part(txt,2:length(txt)-1)
endfunction
