function h =  findobj(propertyName, propertyValue)
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

currentFig = gcf();

// Return value
h = []

propertyName = convstr(propertyName,"l");

// Get all opened figures
figureIds = winsid();

// Iterate over all figures
for figureindex = 1:size(figureIds,2)
  
  // Get the childrens
  f=scf(figureIds(figureindex));
  children = f.children
  
  // Iterate over childrens
  for childIndex = 1:size(children,1)
    
    if children(childIndex).type == "uimenu" then
      if isAnUimenuProperty(propertyName) then
	if isequal(get(children(childIndex), propertyName), propertyValue) then
	  h = children(childIndex);
	  scf(currentFig);
	  return
	end
      end
    elseif children(childIndex).type == "uicontrol" then
      if isAnUicontrolProperty(propertyName) then
	if isequal(get(children(childIndex), propertyName), propertyValue) then
	  h = children(childIndex);
	  scf(currentFig);
	  return
	end
      end
    
    end
  end
end
scf(currentFig);
endfunction
//-----------------------------------------------------------------------------
function res = isAnUimenuProperty(propertyName)
// Copyright INRIA 2008
// Vincent COUVERT
uimenuProperties = ["callback","label","tag","foregroundcolor"];
res = or(propertyName==uimenuProperties);
endfunction
//-----------------------------------------------------------------------------
function res = isAnUicontrolProperty(propertyName)
// Copyright INRIA 2008
// Vincent COUVERT
uicontrolProperties = ["backgroundcolor",
    "callback",
    "fontangle",
    "fontsize",
    "fontunits",
    "fontweight",
    "fontname",
    "foregroundcolor",
    "horizontalalignment",
    "listboxtop",
    "max",
    "min",
    "parent",
    "position",
    "relief",
    "sliderstep",
    "string",
    "style",
    "tag",
    "units",
    "userdata",
    "value",
    "verticalalignment"];
res = or(propertyName==uicontrolProperties);
endfunction
//-----------------------------------------------------------------------------
