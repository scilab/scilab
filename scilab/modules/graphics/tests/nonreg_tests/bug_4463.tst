// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4463 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4463
//
// <-- Short Description -->
// Creating an one-colored Sgrayplot generate some exceptions and does not plot anything.
// 

x=-10:10; y=-10:10; m = zeros(21,21);
for ix=1:21
  for iy=1:21
         m(ix,iy)=1;
   end;
end;
cmap=[1 1 1
0 0 0];
f=gcf(); f.color_map=cmap;
Sgrayplot(x,y,m,"111",[-10,-10,10,10])

// should not display any warinign or error

