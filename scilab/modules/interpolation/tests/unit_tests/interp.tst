// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

x = linspace(0,1,11)';
y = cosh(x-0.5);
d = splin(x,y);
xx = linspace(-0.5,1.5,401)';
yy0 = interp(xx,x,y,d,"C0");
if or(size(yy0)<> [401 1]) then pause,end
if or(yy0 > 2) then pause,end
if or(yy0 < 0) then pause,end


yy1 = interp(xx,x,y,d,"linear");
if or(size(yy1)<> [401 1]) then pause,end
if or(yy1 > 2) then pause,end
if or(yy1 < 0) then pause,end


yy2 = interp(xx,x,y,d,"natural");
if or(size(yy2)<> [401 1]) then pause,end
if or(yy2 > 2) then pause,end
if or(yy2 < 0) then pause,end

yy3 = interp(xx,x,y,d,"periodic");  
if or(size(yy3)<> [401 1]) then pause,end
if or(yy3 > 2) then pause,end
if or(yy3 < 0) then pause,end