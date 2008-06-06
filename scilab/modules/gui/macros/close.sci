function close(h)
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Close a graphic figure or delete an uimenu or an uicontrol
// This function has been written for compatibility with old TK uicontrols/uimenus

// handle has been given ?
rhs=argn(2);

if rhs < 1 then
  // No handle given so destroy current figure
  if ~isempty(winsid()) then
    delete(gcf());
  end
else 
  if type(h) == 9 then // Graphics handle
    delete(h);
  elseif type(h) == 1 then // Assumed to be a old TK figure
    ierr = execstr("figureExists = ~isempty(find(winsid()==get(h,""figure_id"")))", "errcatch");
    if ~ierr & figureExists then
      delete(scf(h));
    end
  else
    // Do not know what to do
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A graphic handle or a real expected.\n"), "close", 1));
  end
end
endfunction

