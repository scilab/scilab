function answ=edit_error(clearerror)
  if argn(2)==0 then clearerror=%f; end
  [str,n,line,func]=lasterror(clearerror)
  if n>0 then
    if func=="" then
      answ=gettext('messages','the last error did not occur inside a library function.');
    else
      p=get_function_path(func);
      if p<>[] then
        scipad(p);
        TCL_EvalStr("blinkline "+string(line)+" "+func,"scipad");
        answ=gettext('messages','opening file ')+p+gettext('messages',' on line ')+string(line)+gettext('messages','the source file of the function ')+func;
      else
        answ=gettext('messages','the source file of the function ')+func+ ..
              gettext('messages',' was not found in any library path');
      end
    end
  else
    answ=gettext('messages',' was not found in any library path');
  end
endfunction

