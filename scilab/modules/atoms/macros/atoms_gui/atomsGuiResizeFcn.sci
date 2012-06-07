// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function atomsGuiResizeFcn()

atomsfig = findobj("tag","atomsFigure");
atomsfigPosition = atomsfig.axes_size;
figwidth     = atomsfigPosition(1);
figheight    = atomsfigPosition(2);

margin       = 10;
widgetHeight   = 25;
msgHeight    = 30;
buttonHeight   = 20;

listboxWidth        = 200;
listboxFrameWidth     = listboxWidth + 2*margin;
listboxFrameHeight    = figheight- 3*margin - msgHeight;
listboxHeight       = listboxFrameHeight - 2*margin;

descFrameWidth       = figwidth - listboxFrameWidth - 3*margin;
descFrameHeight      = listboxFrameHeight;
descWidth        = descFrameWidth  - 2*margin;
descHeight         = descFrameHeight - 4*margin - buttonHeight;

msgWidth     = figwidth -2*margin;

// Test for objects creation
msgText = findobj("Tag", "msgText"); // Last uicontrol to be created
if isempty(msgText) then
    return
end

// Frame
LeftFrame = findobj("Tag", "LeftFrame");
set(LeftFrame, "Position", [margin widgetHeight+2*margin listboxFrameWidth listboxFrameHeight]);

// Listbox
LeftListbox = findobj("Tag", "LeftListbox");
set(LeftListbox, "Position", [ margin margin listboxWidth listboxHeight]);

// Frame
DescFrame = findobj("Tag", "DescFrame");
set(DescFrame, "Position", [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight]);

// Frame title
DescTitle = findobj("Tag", "DescTitle");
set(DescTitle, "Position", [2*margin descFrameHeight-1.5*margin 200 widgetHeight]);

// Details of a module
Desc = findobj("Tag", "Desc");
set(Desc, "Position", [ margin margin+buttonHeight+2*margin descWidth descHeight]);

// Buttons
// -------------------------------------------------------------------------

buttonWidth = (descFrameWidth - 4*margin) / 3;

// "Remove" Button
removeButton = findobj("Tag", "removeButton");
set(removeButton, "Position", [margin margin buttonWidth widgetHeight]);

// "Install" Button
installButton = findobj("Tag", "installButton");
set(installButton, "Position", [buttonWidth+2*margin margin buttonWidth widgetHeight]);

// "Update" Button
updateButton = findobj("Tag", "updateButton");
set(updateButton, "Position", [2*buttonWidth+3*margin margin buttonWidth widgetHeight]);

// Installed Modules: List of installed modules
// =========================================================================

descWidth        = descFrameWidth  - 2*margin;
descHeight         = descFrameHeight - 3*margin;

// Frame
HomeFrame = findobj("Tag", "HomeFrame");
set(HomeFrame, "Position", [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight]);

// Frame title
HomeTitle = findobj("Tag", "HomeTitle");
set(HomeTitle, "Position", [2*margin descFrameHeight-1.5*margin 200 widgetHeight]);

// Home
HomeListbox = findobj("Tag", "HomeListbox");
set(HomeListbox, "Position", [ margin margin descWidth descHeight]);

// Message Frame
// =========================================================================

// Frame
msgFrame = findobj("Tag", "msgFrame");
set(msgFrame, "Position", [margin margin msgWidth msgHeight]);

// Text
msgText = findobj("Tag", "msgText")
set(msgText, "Position", [2 2 msgWidth-10 msgHeight-4]);

endfunction
