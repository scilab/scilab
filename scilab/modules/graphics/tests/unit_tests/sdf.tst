// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// non regression bug for sdf

// used to open window number 0
// and crash scilab
  
   x=[0:0.1:2*%pi]';
   f=get("default_figure"); // get the handle of the model figure 
     // setting its' properties
   f.background=4; // figure 0 was created here
   for (i=1:6)
     xset("window",i) // create a figure with the identifier i
     plot2d(x,[sin(x) cos(x)],[i -i])
     xclick();
     if i == 4, sdf(); end // return to the  default values of the figure's model
    end
    



