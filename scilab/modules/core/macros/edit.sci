// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2008 - INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function edit(macroname,ueditor)
// macroname : character string giving a macroname
// ueditor : external command : no more supported
//
  
  if (~isdef('editor') & (funptr('editor')==0))  then
    warning(sprintf(gettext("%s: Requires scilab editor.\n"),"edit"));
    return
  end
  
  [lhs,rhs]=argn(0);
  if (rhs > 1) then 
    error(sprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"edit",1));
  end
  
  finded=%f;tmp=%f
  // tmpdir will have trailing / or \
  tmpdir= pathconvert(TMPDIR);

  if rhs == 1 then // macroname is given
    tmpfile= tmpdir+macroname+'.sci';
    if funptr(macroname)<>0 then
      error(msprintf(gettext("%s: %s is a uneditable hard coded function.\n"),"edit",macroname));
    end
    libr = whereis(macroname);
    if libr<>[] then // macroname is the name of a defined function
      path = string(evstr(libr));
      path = path(1);
      // convert path according to MSDOS value and expand SCI
      path = pathconvert(path);
      fname = path+macroname+'.sci';
      finded = %t;
    elseif isdef(macroname)
       if typeof(evstr(macroname))=='function' then
        // tour de force to keep the original function name
        execstr("txt=tree2code(macr2tree("+macroname+"),%t)")
        fname = tmpfile;
        mputl(txt,fname);
        finded = %t;
       end
    end
  else //no macroname specified
    macroname = 'untitled';
    tmpfile = tmpdir+macroname+'.sci';
    finded = %f;
  end

  if ~finded then // macroname is the name of an undefined function
    fname = tmpfile;
    txt=['function []='+macroname+'()';'endfunction'];
    mputl(txt,fname);
  end

  // call the editor with the filename
  editor(fname)

endfunction
