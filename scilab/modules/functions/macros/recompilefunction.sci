// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function recompilefunction(funname,kind,force)
  if ~exists("force","local") then force=%f; end
  if ~exists("kind","local") then kind="c"; end
  if ~exists(funname)
     error(msprintf(gettext("No variable named: %s"),funname))
  end
  clear fvar funtext tempfun
  execstr("fvar="+funname)
  if ~or(type(fvar)==[11 13]) then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: A Scilab function name is expected.\n"),"recompilefunction",1))
  end
  if type(fvar)==11 & ~force then 
    oldkind="n"
    if kind=="n" then 
      warning(msprintf(gettext("%s is already noncompiled, nothing to do!"),funname))
      return
    end
//can't avoid "Warning: redefining function: fvar", sorry
//    if kind=="c" then comp(fvar); end 
//    if kind=="p" then comp(fvar,2); end
//    execstr(funname+"=resume(fvar)")
//or:
    [out,in,funtext]=string(fvar);
    deff("["+strcat(out,",")+"]=tempfun("+strcat(in,",")+")",..
          funtext,kind)
    execstr(funname+"=resume(tempfun)")
  elseif type(fvar)==13 then
    lst=macr2lst(fvar)
    if lst(5)(1)=="25" then oldkind="p"; else oldkind="c"; end
    if kind=="c" & oldkind=="c" & ~force then 
      warning(msprintf(gettext("%s is already compiled, nothing to do!"),funname))
      return
    end
    if kind=="p" & oldkind=="p" & ~force then 
      warning(msprintf(gettext("%s is already compiled for profiling, nothing to do!"),funname))
      return
    end
    funtext=fun2string(lst,"tempfun")
    deff(strsubst(funtext(1),"function ",""),funtext(2:$-1),kind)
    execstr(funname+"=resume(tempfun)")
  end
endfunction
