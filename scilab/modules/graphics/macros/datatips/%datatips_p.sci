// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %datatips_p(d)
  fun=fun2string(d.formatfunction)
  
  F=getfield(1,d);
  txt=[];
  L=max(length(F(2:$)))
  for k=2:size(F,'*')
    f=F(k)
    if f=="interpolate" then
      txt=[txt;
           part(f,1:L+1)+'= '+sci2exp(d.interpolate)]
    elseif  f=="replace" then
       txt=[txt;
           part(f,1:L+1)+'= '+sci2exp(d.replace)]
    elseif  f=="formatfunction" then
       txt=[txt;
           part(f,1:L+1)+': '
           '  '+fun2string(d.formatfunction)]
    elseif f=="tips" then
      txt=[txt;
           part(f,1:L+1)+': '+string(size(d.tips,'*'))+ " tip handles" ]
    else
      txt=[txt
           part(f,1:L+1)+': '+'['+strcat(string(size(d(f))),'x')+' '+typeof(d(f))+']']
    end
  end
  mprintf("  %s\n",txt)
endfunction

