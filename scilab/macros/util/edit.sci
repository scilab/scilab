function res=edit(macroname,editor)
// macroname : character string giving a macroname 
//
// Copyright INRIA
  default_editor="emacs -i -geometry 80x50+427+143  -font 9x15 "
  [lhs,rhs]=argn(0)
  //
  finded=%f;tmp=%f

  if MSDOS then 
    write(%io(2),'edit: Not implemented on win32');
    res=evstr(macroname);
    return;
  end

  if rhs>=1 then // macroname is given
    if funptr(macroname)<>0 then
      error(macroname+'is a uneditable hard coded function,')
    end
    libr=whereis(macroname)
    if libr<>[] then // macroname is the name of a defined function
      w=string(evstr(libr));w=w(1)
      if part(w,1:4)=='SCI/' then //substitute SCI/ with the scilab path
	w=SCI+'/'+part(w,5:length(w))
      end
      //if file is not writable create a copy in TMPDIR
      rep=unix_g("if [ -w '+w+macroname+'.sci '+' ]; then echo ok ;else echo nok; fi")
      if part(rep,1:2)=='ok' then
	fname=w+macroname+'.sci'
      else
	fname=TMPDIR+'/'+macroname+'.sci'
	unix_s("cp "+w+macroname+'.sci '+fname+'; chmod +w '+fname )
	tmp=%t
      end
      finded=%t
    end  
  else //no macroname specified
    macroname='untitled', 
    finded=%f
  end

  if ~finded then // macroname is the name of an undefined function
    comment='/'+'/'
    fname=TMPDIR+'/'+macroname+'.sci'
    head='function []='+macroname+'()'
    execstr('write(fname,head)','errcatch')
    tmp=%t
  end
  // call the editor with the filename
  if rhs<=1, editor =default_editor ;end
  unix_s(editor+' '+fname);

  //load the macro in scilab
  if tmp then write(%io(2),'modified file may be found in '+fname),end 
  getf(fname,'c')
  //return the loaded variable
  res=evstr(macroname);
endfunction
