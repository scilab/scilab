// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function repm=mtlb_repmat(M,m,n)
// Emulation function for repmat() Matlab function

str=%f
if type(M)==10 then
  str=%t
  M=asciimat(M)
end

if or(type(M)==[4,6]) then
  M=bool2s(M)
end

rhs=argn(2)

// repmat(M,m) -> repmat(M,m,m)
if rhs==2 then
  n=m
end
// repmat(M,[m,n,p,...])
if rhs==2 & size(m,"*")<>1 then
  tmp=list()
  for k=1:size(m,"*")
    tmp(k)=m(k)
  end
  repm=ones(tmp(1:$)).*.M
// repmat(M,m,n)
else
  if or(type(n)==[4,6]) then
    n=bool2s(n)
  end
  repm=ones(m,n).*.M
end
if str then
  repm=asciimat(repm)
end
endfunction

