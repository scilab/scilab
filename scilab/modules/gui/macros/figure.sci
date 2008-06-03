// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version of uicontrols)
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function h=figure(varargin)

Rhs=length(varargin);

if (Rhs == 0) then
  // Create a new graphic window
  h=scf();
  // Set its default background for compatibility with versions < 5.0
  set(h,"background",addcolor([0.8 0.8 0.8]))
else
  if (Rhs == 1) then
    
    if size(varargin(1), "*")==1 then
      // Create a new graphic window or set it (if already exists)
      h = createOrSetFigure(varargin(1));
    else
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A ''Figure'' handle or a real expected.\n")), "figure", 1);
    end
    
  else
    
    if (modulo(Rhs,2)==0) then
      // Even number of input arguments

      // Create a new graphic window or set it (if already exists)
      h = createOrSetFigure([]);
	
      // Set all properties
      for i=1:2:Rhs
	set(h, getFigureProperty(varargin(i)), getPropertyValue(varargin(i),varargin(i+1)));
      end

    else
      // Odd number of input arguments
      if size(varargin(1), "*")==1 then
	// Create a new graphic window or set it (if already exists)
	h = createOrSetFigure(varargin(1));

	// Set all properties
	for i=2:2:Rhs
	  set(h, getFigureProperty(varargin(i)), getPropertyValue(varargin(i),varargin(i+1)));
	end
      else
	error(msprintf(gettext("%s: Wrong size for input argument #%d: A ''Figure'' handle or a real expected.\n")), "figure", 1);
      end
    end
  end
end

endfunction
// -----------------------------------------------------------

// -----------------------------------------------------------
function h = createOrSetFigure(numOrHandle)
// Vincent COUVERT - INRIA 2008
// This function create a figure or set if if already exists
// The figure background is set to default for new figures

if isempty(numOrHandle)
  alreadyExists = %F;
elseif type(numOrHandle)==1 // Single value
  alreadyExists = ~isempty(find(winsid()==numOrHandle));
else // Handle
  alreadyExists = ~isempty(find(winsid()==get(numOrHandle,"figure_id")));
end

if isempty(numOrHandle)
  h = scf();
else
  h=scf(numOrHandle);
end

if ~alreadyExists
  // Set the default background if figure has just been created (compatibility with versions < 5.0)
  set(h,"background",addcolor([0.8 0.8 0.8]))
end
// Hide the current axis
set(gca(),"visible","off");
endfunction
// -----------------------------------------------------------

// -----------------------------------------------------------
function sciFigProperty = getFigureProperty(tkFigProperty)
// Vincent COUVERT - INRIA 2008
// Get the Java figure property corresponding to the TK figure property

if strcmpi(tkFigProperty, "backgroundcolor") == 0 then
  sciFigProperty = "background";
elseif strcmpi(tkFigProperty, "foregroundcolor") == 0 then
  sciFigProperty = "foreground";
else
  sciFigProperty = tkFigProperty;
end
endfunction
// -----------------------------------------------------------

// -----------------------------------------------------------
function sciPropertyValue = getPropertyValue(tkFigProperty, tkPropertyValue)
// Vincent COUVERT - INRIA 2008
// Get the Java property value corresponding to the TK property value

if strcmpi(tkFigProperty, "backgroundcolor") == 0 then
  sciPropertyValue = addcolor(tkPropertyValue);
elseif strcmpi(tkFigProperty, "foregroundcolor") == 0 then
  sciPropertyValue = addcolor(tkPropertyValue);
else
  sciPropertyValue = tkPropertyValue;
end
endfunction
// -----------------------------------------------------------
