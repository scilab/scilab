// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [stk,top]=str2f(ch,stk)
// traduit la definition d'une chaine de caracteres
//!
quote=''''
nch=length(ch)
k=1
while k<=nch
  if part(ch,k)==quote then
    ch=part(ch,1:k)+quote+part(ch,k+1:nch)
    nch=nch+1;k=k+1
  end
  k=k+1
end
top=top+1
stk(top)=list(quote+ch+quote,'-3','10','1','1')
endfunction
