// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Draw a simplex along the axes in 2D
x = zeros(4,1)
y = zeros(4,1)

// First point is (0,0)
// Second point is (1,0)
x(2) = 1.0
y(2) = 0
// Third point is (0,2)
x(3) = 0.0
y(3) = 2.0
// Fourth point is (0,0), just to make the loop in the simplex
// Now plot the simplex
plot(x,y)
f = gcf()
f.children.children.children.thickness = 4
f.children.children.children.mark_style = 9
f.children.children.children.mark_foreground = 3
f.children.children.children.mark_size = 10

