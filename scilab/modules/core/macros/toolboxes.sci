// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//===========================================================
function [y] = toolboxes(path,flag,force)
// INTERNAL macro should NOT used by users
// toolboxes loading
// path is a directory to explore for contribs
// flag & force removed never managed
//===========================================================
  global %toolboxes
  global %toolboxes_dir
//===========================================================
  [lhs,rhs] = argn(0) 
  y = [];
  if (rhs == 1) & typeof(path)=="constant" then 
    // return string to exec
    y = 'exec(""" + %toolboxes_dir + %toolboxes(path) + filesep() + "loader.sce" + """);';
    return 
  end
    
  if rhs == 0 then 
    path = SCI + filesep() + 'contrib';
  end
  
  if rhs > 1 then
    // Remove flag & force input arguments
    warnobsolete("toolboxes", "5.3")
  end

  cur_wd = pwd();
  chdir(path);
  files = listfiles('.');
  contribs = [];
  for k = 1:size(files,'*') 
    if fileinfo(files(k)+'/loader.sce') <> [] then 
      contribs = [contribs ; files(k)];
    end
  end

  if (contribs <> []) & (getscilabmode() == 'STD') then 
    delmenu(gettext("&Toolboxes"));
    h = uimenu("parent", 0, "label", gettext("&Toolboxes"));
    for k=1:size(contribs,'*')
      m = uimenu(h,'label', string(contribs(k)), 'callback','execstr(toolboxes('+string(k)+'))');
    end
    unsetmenu(gettext("&Toolboxes"));
  end
  
  %toolboxes = contribs;
  %toolboxes_dir = pathconvert(path);
  chdir(cur_wd);
  
endfunction
//===========================================================
