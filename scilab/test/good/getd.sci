// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function getd(path,option)
  
// get all .sci files in the specified directory
  
  [lhs,rhs]=argn(0);
  
  if rhs<=0 then
    path='./';
  end
  
  // convert path according to getos() == 'Windows' flag
  // and with env var substitutions
  path = pathconvert(path,%t,%t);
  
  // list the sci files
  lst          = listfiles(path+"*.sci",%f);
  lst_filtered = [];

  // remove wrong files extension
  // bug 2289

  for i=1:size(lst,"*")
    if( regexp(lst(i),"/.sci$/") <> [] ) then
      lst_filtered = [lst_filtered;lst(i)];
    end
  end

  lst = lst_filtered;
  clear lst_filtered;

  if lst==[] | lst== "" then
    error(msprintf(gettext("%s: I cannot find any files with extension %s in %s\n"),"getd",".sci",path));
    return ;
  end
  
  nold = size(who('get'),'*')
  //prot = funcprot();funcprot(0)
  
  for k=1:size(lst,'*'); 
    if fileparts(lst(k),"extension")==".sci" then
      if execstr("exec(lst(k));","errcatch")<>0 then
        warning(msprintf(gettext("%s: Incorrect function in file %s.\n"),"getd",lst(k)))
      end
    end
  end
  
  //funcprot(prot);
  new = who('get');
  new = new(1:(size(new,'*')-nold-2));  // -4 becomes -2: fix the fix for bug 2807
  if new<>[] then
    execstr('['+strcat(new,',')+']=resume('+strcat(new,',')+')')
  end
  
endfunction
