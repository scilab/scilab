function res=edit(macroname,editor)
// macroname : character string giving a macroname 
//
// Copyright INRIA
  default_editor="emacs -i -geometry 80x50+427+143  -font 9x15 "
  [lhs,rhs]=argn(0)
  //
  finded=%f;tmp=%f
  // tmpdir will have trailing / or \
  tmpdir= pathconvert(TMPDIR);
  tmpfile= tmpdir+macroname+'.sci';
  if MSDOS then 
    default_editor="emacs ";
  end

  if rhs>=1 then // macroname is given
    if funptr(macroname)<>0 then
      error(macroname+'is a uneditable hard coded function,')
    end
    libr=whereis(macroname)
    if libr<>[] then // macroname is the name of a defined function
      path=string(evstr(libr));
      path=path(1)
      // convert path according to MSDOS value and expand SCI
      path=pathconvert(path);
      fname= path+macroname+'.sci';
      // check if writable 
      // if MSDOS is true we assume here that file is writable
      if ~MSDOS then 
	rep=unix_g("if [ -w "+ fname +" ]; then echo ok ;else echo nok; fi")
	if part(rep,1:3)=='nok' then
	  //if file is not writable create a copy in TMPDIR
	  //unix_s("cp "+ fname + " " + tmpfile ); fname=tmpfile;
	  txt = mgetl(fname,-1);fname=tmpfile; mputl(txt,fname);
	  tmp=%t
	end 
      end
      finded=%t
    elseif isdef(macroname) 
       if typeof(macroname)=='function' then 
	 // macro name was defined online 
	 txt = fun2string(evstr(macroname)) 
	 fname=tmpfile
	 mputl([txt,'endfunction'],fname);
	 tmp=%t
	 finded=%t
       end
    end
  else //no macroname specified
    macroname='untitled', 
    finded=%f
  end

  if ~finded then // macroname is the name of an undefined function
    fname=tmpfile
    txt=['function []='+macroname+'()';'endfunction'];
    mputl(txt,fname);
    tmp=%t
  end

  // call the editor with the filename
  if rhs<=1, editor =default_editor ;end
  if MSDOS then 
     // white spaces in path 
     unix_s(editor+' ""'+fname+'""');
  else
     unix_s(editor+' '+fname);
  end
  //load the macro in scilab
  if tmp then write(%io(2),'modified file may be found in '+fname),end 
  getf(fname,'c')
  //return the loaded variable
  res=evstr(macroname);
endfunction
