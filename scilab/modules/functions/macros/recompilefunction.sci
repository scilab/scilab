function recompilefunction(funname,kind,force)
  if ~exists("force","local") then force=%f; end
  if ~exists("kind","local") then kind="c"; end
  if ~exists(funname)
     error("No variable named: "+funname)
  end
  clear fvar funtext tempfun
  execstr("fvar="+funname)
  if ~or(type(fvar)==[11 13]) then
     error(funname+" must be the name of a scilab function variable")
  end
  if type(fvar)==11 & ~force then 
    oldkind="n"
    if kind=="n" then 
      warning(funname+" is already noncompiled, nothing to do!")
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
      warning(funname+" is already compiled, nothing to do!")
      return
    end
    if kind=="p" & oldkind=="p" & ~force then 
      warning(funname+" is already compiled for profiling, nothing to do!")
      return
    end
    funtext=fun2string(lst,"tempfun")
    deff(strsubst(funtext(1),"function ",""),funtext(2:$-1),kind)
    execstr(funname+"=resume(tempfun)")
  end
endfunction
