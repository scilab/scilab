
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %fun=createfun(t)
  //given t a string vector where first line gives the function calling
  //sequence and the following the function instructions, it returns et
  //Scilab function.
  // This is  mainly used by sci2exp
  h=t(1)
  k=strindex(h,"=")
  if k==[] then k1=1,else k1=k(1),end
  k=strindex(h,"(")
  if k==[] then k2=length(h),else k2=k(1),end
  h=part(h,1:k1)+"%fun"+part(h,k2:length(h))
  deff(h,t(2:$)),
endfunction
