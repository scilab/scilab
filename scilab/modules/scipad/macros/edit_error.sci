function answ=edit_error(clearerror)
  if argn(2)==0 then clearerror=%f; end
  [str,n,line,func]=lasterror(clearerror)
  if n>0 then
    if func=="" then
      answ=gettext('messages','scipad_message_8');
    else
      p=get_function_path(func);
      if p<>[] then
        scipad(p);
        TCL_EvalStr("blinkline "+string(line)+" "+func,"scipad");
        answ=gettext('messages','scipad_message_9')+p+gettext('messages','scipad_message_10')+string(line)+gettext('messages','scipad_message_11')+func;
      else
        answ=gettext('messages','scipad_message_11')+func+ ..
              gettext('messages','scipad_message_12');
      end
    end
  else
    answ=gettext('messages','scipad_message_12');
  end
endfunction

