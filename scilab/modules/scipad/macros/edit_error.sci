function answ=edit_error(clearerror)
  if argn(2)==0 then clearerror=%f; end
  [str,n,line,func]=lasterror(clearerror)
  if n>0 then
    if func=="" then
      answ=gettext('the last error did not occur inside a library function.');
    else
      p=get_function_path(func);
      if p<>[] then
        scipad(p);
        TCL_EvalStr("blinkline "+string(line)+" "+func,"scipad");
        answ=gettext('opening file ')+p+gettext(' on line ')+string(line)+gettext('the source file of the function ')+func;
      else
        answ=gettext('the source file of the function ')+func+ ..
              gettext(' was not found in any library path');
      end
    end
  else
    answ=gettext(' was not found in any library path');
  end
endfunction

