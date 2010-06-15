// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Simple wrapper around xmltojar
function tbx_build_help(toolbox_name, help_lang_path)
  if(~exists(help_lang_path, "local"))
    help_lang_path = pwd();
  end

  if or(getscilabmode() == ["STD" "NW"]) then
    xmltojar(help_lang_path, toolbox_name);
  else
    warning(msprintf(gettext("%s: documentation cannot be built in this scilab mode: %s.\n"), ..
            "tbx_build_help", getscilabmode()));
  end
endfunction

