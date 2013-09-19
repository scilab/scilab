// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste SILVY 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- INTERACTIVE TEST -->

// all the kind of uicontrols
uicontrolNames = ["pushbutton", "radiobutton", "checkbox", "edit", "text", "slider", "frame", "listbox", "popupmenu"];
nbNames = size(uicontrolNames, '*');

// use GLCanvas
usecanvas(%t);


f0 = scf();
// plot something
plot;
figSize = f0.axes_size;

uicontrolSize = [1/(nbNames + 1) * figSize(1), 1/(nbNames + 1) * figSize(2) * 0.5];
uicontrolPosIncr = [1/(nbNames + 1), 1/(nbNames + 1)] .* figSize;

for i = 1:nbNames
  // create only the uicontrol
  uicontrolPos(i,:) = [i * uicontrolPosIncr, uicontrolSize];
  uicontrol(f0, 'style',uicontrolNames(i), 'position',uicontrolPos(i,:), 'SliderStep',[1,2],..
            'String',"Scilab", 'Min',0, 'Max',10, 'units','pixels', 'callback','disp(''Scilab'');');
end

f1 = scf();
f1.axes_size = f0.axes_size; // to be sure
// plot something
plot3d;

for i = 1:nbNames
  // create uicontrol enclosed in a frame
  // create the frame
  uicontrol(f1, 'style','frame', 'position',uicontrolPos(i,:), 'background',[1 1 1]);
  // and the usicontrol with same position and dimensions
  uicontrol(f1, 'style',uicontrolNames(i), 'position',uicontrolPos(i,:), 'SliderStep',[1,2],..
            'String',"Scilab", 'Min',0, 'Max',10, 'units','pixels', 'callback','disp(''Scilab'');');
end

// check in each figures that uicontrols appear
