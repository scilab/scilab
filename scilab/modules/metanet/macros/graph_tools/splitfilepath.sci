
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [path,name,ext]=splitfilepath(fname)

  l=length(fname)
  //getting the extension part
  n=l
  while n>0
    cn=part(fname,n)
    if cn=='.'|cn=='/'|cn=='\' then break,end
    n=n-1
  end
  if n==0 then
    ext=emptystr()
  elseif cn=='/'|cn=='\' then
    ext=emptystr()
    n=l
  else
    ext=part(fname,n+1:l)
    n=n-1
  end
  //getting the name part
  l=n
  n=l
  while n>0
    cn=part(fname,n)
    if cn=='/'|cn=='\' then break,end
    n=n-1
  end
  if n==0 then
    name=part(fname,1:l)
    path='./'
  else
    name=part(fname,n+1:l)
    path=part(fname,1:n)
  end
endfunction
