// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %l_p(s)
  str=' '
  select type(s)
  case 16 then
    f=getfield(1,s),1,-1
    first=2
    write(%io(2),gettext('   tlist of type ')+f(1)+gettext(' with fields:'))
  case 17 then
    f=getfield(1,s),1,-1
    first=2
    write(%io(2),gettext('   mlist of type ')+f(1)+gettext(' with fields:'))
  else
    f=[]
    first=1
    write(%io(2),gettext('   list with entries:'))
  end
  ll=lines()
  nf=size(f,'*');ns=lstsize(s)
  for k=first:max(nf,ns)
    if k<=nf then 
      var=f(k),
      value=s(var)
      label=var
    else 
      var=k,
      if k<=ns then value=s(k),else value='Undefined',end
      label=string(k)
    end
    txt='    '+label+': '
    if or(type(value)==[130 9])
      txt=txt+'['+strcat(string(size(value)),'x')+' '+typeof(value)+']'
    else
      str=sci2exp(value,ll(1))
      if size(str,'*')==1 then
	txt=txt+str
      else
	txt=txt+'['+strcat(string(size(value)),'x')+' '+typeof(value)+']'
      end
    end
    write(%io(2),txt,'(a)')
  end
endfunction
