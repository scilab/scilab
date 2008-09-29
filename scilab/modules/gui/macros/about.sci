// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function about(h)
// Displays a dialog about Scilab
// Used from menus "About Scilab..."

[a, b] = getversion();

btn = messagebox(msprintf(gettext("Scilab 5.0 and future versions are released under the terms of the CeCILL license.\n") +..
    gettext("Some Scilab modules are released under the GPLv2 license.\n\n") +..
    gettext("Authors:\nScilab Consortium / INRIA / ENPC / Contributors\n\n") +..
    gettext("Version %s (%s)\n\n") +..
    gettext("http://www.scilab.org/"), a, b($-1)), gettext("About Scilab..."), "scilab", [gettext("Acknowledgements"), gettext("Ok")], "modal");

if btn==1 then
  if ~isempty(fileinfo("SCI/ACKNOWLEDGEMENTS")) then // Windows and Linux source version
    txt = mgetl("SCI/ACKNOWLEDGEMENTS");
  elseif ~isempty(fileinfo("SCI/../../ACKNOWLEDGEMENTS")) then // Linux binary version
    txt = mgetl("SCI/../../ACKNOWLEDGEMENTS");
  else
    return
  end
  messagebox(txt, gettext("Scilab Contributors..."), "modal");
end
endfunction

