// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- TEST WITH GRAPHIC -->

initialfonts = xlfont();
if size(initialfonts,'*') <> 11 then pause,end

availablefonts = xlfont('AVAILABLE_FONTS');
if size(availablefonts,'*') < 11 then pause,end

for i=1:11:1
  xlfont(availablefonts(i),i);
end

for i=1:11:1
  xlfont(availablefonts(i),i,%t,%f);
end

for i=1:11:1
  xlfont(availablefonts(i),i,%t);
end

xlfont('reset');

afterresetfonts = xlfont();
if ~and(initialfonts == afterresetfonts) then pause,end


