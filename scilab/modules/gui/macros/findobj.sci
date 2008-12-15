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

  %ierr = execstr("%bool = isequal(get(f, propertyName), propertyValue);", "errcatch");
  if %ierr==0 & %bool then
    h = f;
    return
  end
  
  answ = findMatchingChild(f.children, propertyName, propertyValue);
  if ~isempty(answ) then
    h = answ;
    scf(currentFig);
    return
  end      
end
scf(currentFig);
endfunction
//-----------------------------------------------------------------------------
function hFound = findMatchingChild(children, propertyName, propertyValue)
// Copyright DIGITEO 2008
// Vincent COUVERT

hFound = []

for childIndex = 1:size(children,1)
  
  %ierr = execstr("%bool = isequal(get(children(childIndex), propertyName), propertyValue);", "errcatch");
  if %ierr==0 & %bool then
    hFound = children(childIndex);
    return
  end
  
  %ierr = execstr("get(children(childIndex), ""children"");", "errcatch"); // Does the child have a children property
  
  if %ierr==0 then // Yes the child can have children
    answ = findMatchingChild(children(childIndex).children, propertyName, propertyValue);
    if ~isempty(answ) then
      hFound = answ;
      return
    end
  end
end

endfunction
//-----------------------------------------------------------------------------
