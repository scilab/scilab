// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// Copyright (C) 2008 - DIGITEO - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function h =  findobj(propertyName, propertyValue)

currentFig = gcf();

// Return value
h = []

propertyName = convstr(propertyName,"l");

// Get all opened figures
figureIds = winsid();

// Iterate over all figures
for figureindex = 1:size(figureIds,2)
  
  // Does the figure match the propertyName/propertyValue set ?
  f=scf(figureIds(figureindex));
  // TODO: create a function which lists figure properties to avoid error messages
  %ierr = execstr("%bool = isequal(get(f, propertyName), propertyValue);", "errcatch");
  if %ierr==0 & %bool then
    h = f;
    return
  end
  
  // Iterate over childrens
  children = f.children
  for childIndex = 1:size(children,1)
    
    if children(childIndex).type == "uimenu" then
      answ = findUimenuWithproperty(children(childIndex), propertyName, propertyValue);
      if ~isempty(answ) then
	h = answ;
	scf(currentFig);
	return
      end      
    elseif children(childIndex).type == "uicontrol" then
      answ = findUicontWithproperty(children(childIndex), propertyName, propertyValue);
      if ~isempty(answ) then
	h = answ;
	scf(currentFig);
	return
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
uimenuProperties = ["callback",
    "enable",
    "foregroundcolor",
    "label",
    "tag",
    "visible"];
res = or(propertyName==uimenuProperties);
endfunction
//-----------------------------------------------------------------------------
function res = isAnUicontrolProperty(propertyName)
// Copyright INRIA 2008
// Vincent COUVERT
uicontrolProperties = ["backgroundcolor",
    "callback",
    "enable",
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
    "verticalalignment",
    "visible"];
res = or(propertyName==uicontrolProperties);
endfunction
//-----------------------------------------------------------------------------
function hFound = findUimenuWithproperty(uimenuHandle, propertyName, propertyValue)
// Copyright DIGITEO 2008
// Vincent COUVERT
hFound = [];
if isAnUimenuProperty(propertyName) then
  if isequal(get(uimenuHandle, propertyName), propertyValue) then
    hFound = uimenuHandle;
    return
  end
end
// Loop over submenus
submenus = get(uimenuHandle, "children");
for submenuIndex = 1:size(submenus,1)
  answ = findUimenuWithproperty(submenus(submenuIndex), propertyName, propertyValue);
  if ~isempty(answ) then
    hFound = answ;
    return
  end
end
endfunction
//-----------------------------------------------------------------------------
function hFound = findUicontWithproperty(uicontrolHandle, propertyName, propertyValue)
// Copyright DIGITEO 2008
// Vincent COUVERT
hFound = [];
if isAnUicontrolProperty(propertyName) then
  if isequal(get(uicontrolHandle, propertyName), propertyValue) then
    hFound = uicontrolHandle;
    return
  end
end
// Loop over children
children = get(uicontrolHandle, "children");
for childIndex = 1:size(children,1)
  answ = findUicontWithproperty(children(childIndex), propertyName, propertyValue);
  if ~isempty(answ) then
    hFound = answ;
    return
  end
end
endfunction
//-----------------------------------------------------------------------------
