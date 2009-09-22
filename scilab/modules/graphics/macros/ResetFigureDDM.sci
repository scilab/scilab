// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// Reset the Default Drawing Mode (DDM) of the figure
// immediate_drawing is set to its input value.
// This function is intended to be private.
function ResetFigureDDM(cur_figure, cur_draw_mode)

if type(cur_figure) == 9
  if cur_figure.type == "Figure"
    cur_figure.immediate_drawing = cur_draw_mode;
  else
    warning("Error in ResetFigureDDM : input argument must be a figure graphic handle");
    return;
  end
end

endfunction
