//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//
// Import a scicos diagram.
//
// This method wrap all the scicos entry points (load, version, etc...).
//
// @source the filename or a diagram instance
// @out the scicos diagram structure (scs_m)
function out = importScicosDiagram(source)
  out = [];
  
  [lhs,rhs] = argn(0);
  if rhs <> 1 then
    error(999,  msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "importScicosDiagram", 1));
    return;
  end
  
  if typeof(source) == 'diagram' then //import scs_m structure
    out = source;    
  elseif typeof(source) == 'string' then //import cos or cosf file
    [path,name,ext] = fileparts(source);
    ext2 = convstr(ext, "u");
    if ext2 == ".COS" then
      load(source);
      out = scs_m;
      clear scs_m;
    elseif ext2 == ".COSF" then
      exec(source, -1); 
      out = scs_m;
      clear scs_m;
    else
      error(999 ,msprintf(_("%s: Wrong type for argument #%d: String or diagram structure expected"), "importScicosDiagram", 1));
      return;
    end
  else
    error(999, msprintf(_("%s: Wrong type for argument #%d: String or diagram structure expected"), "importScicosDiagram", 1));
    return;
  end
  
  version = getDiagramVersion(out);
  out = do_version(out, version);
endfunction

