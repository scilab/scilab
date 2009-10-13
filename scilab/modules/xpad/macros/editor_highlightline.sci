//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function editor_highlightline(filename, linenumber)
if getscilabmode() <> "NWNI" then
  if with_module("xpad") then
    xpad_highlightline(filename, linenumber);
  else
    messagebox(_("Please install xpad module."), _("No text editor available."), "error");
  end
else
  mprintf(gettext('Editor is disabled in this mode; %s.\n'),getscilabmode());
end
endfunction
