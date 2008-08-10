// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xend()
  // export a file using already set fileName and file Type

  global %exportFileName;
  global %driverName;

  if (%exportFileName == []) then
    // filed not already selected : xinit has not been called
    error(msprintf(gettext("%s: %s must be called before any action.\n"),"xend","xinit"));
    return;
  end

  curFig = gcf();
  curFigId = curFig.figure_id;

  // choose export type depending on driver name
  // for "Rec" and "X11" drivers thare actually nothing to do
  select %driverName,
    case "Pos" then
      drawnow();
      xs2ps(curFigId, %exportFileName);
      // delete figure used for export
      delete(curFig);
    case "GIF" then
      drawnow();
      xs2gif(curFigId, %exportFileName);
      // delete figure used for export
      delete(curFig);
    case "PPM" then
      drawnow();
      xs2ppm(curFigId, %exportFileName);
      // delete figure used for export
      delete(curFig);
    case "Fig" then
      drawnow();
      xs2fig(curFigId, %exportFileName);
      // delete figure used for export
      delete(curFig);
  end

  // export done reset exportFileName
  %exportFileName = [];

endfunction

