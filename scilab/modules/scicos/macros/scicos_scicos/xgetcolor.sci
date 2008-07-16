// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function idxColor = xgetcolor(titleString, idxColor)

// XGETCOLOR is a wrapper around UIGETCOLOR and mimics the behavior of TK_GETCOLOR
//
// The GETCOLOR alias defined in SCI/modules/scicos/macros/scicos_scicos/scicos.sci
// initially pointed to TK_GETCOLOR and was in charge of returning an index in a
// colormap.
// UIGETCOLOR, a Java equivalent for TK_GETCOLOR, returns the RGB components of
// the picked color. A wrapper is thus needed to convert these three components
// into the wanted index.
// This way, making GETCOLOR point to XGETCOLOR in scicos.sci (instead of
// TK_GETCOLOR)is enough to keep the same behavior while using a much nicer GUI.
//
// Note: titleString is useless as UIGETCOLOR does not allow to specify a dialog title
  
  // Check I/O arguments
  [lhs, rhs] = argn(0)
  //  - Number:
  if rhs ~= 2
    error(sprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "xgetcolor", 2));
  end
  if lhs ~= 1
    error(sprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"), "xgetcolor", 1));
  end
  //  - Type:
  if typeof(titleString) ~= 'string'
    error(sprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "xgetcolor", 1));
  end
  if typeof(idxColor) ~= 'constant'
    error(sprintf(gettext("%s: Wrong type for input argument #%d: Integer expected.\n"), "xgetcolor", 2));
  end
  //  - Size:
  if size(titleString,'*') ~= 1
    error(sprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"), "xgetcolor", 1));
  end
  if size(idxColor,'*') ~= 1
    error(sprintf(gettext("%s: Wrong size for input argument #%d: Single integer expected.\n"), "xgetcolor", 2));
  end
  

  // Get current colormap and RGB components of the default color (second input argument)
  currentColormap = get(gcf(), "color_map")
  defaultColor = currentColormap(idxColor,:)*255 // colormap: 0..1 ; uigetcolor: 0..255
  
  // Pick a color using UIGETCOLOR instead of a TCL/TK grid and retrieve its RGB components
  rgbColor = uigetcolor(defaultColor)
  if isempty(rgbColor) // user cancelled so quit and do not modify the current color
    return;
  end
  
  // Otherwise, add the picked color at the end of the current colormap and
  // return its index to mimic behavior of TK_GETCOLOR
  idxColor = addcolor(rgbColor/255) // colormap: 0..1 ; uigetcolor: 0..255

endfunction

