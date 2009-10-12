//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("latexsubplot.dem.sce");

// DEMO START

x = 1:0.25:10;
y1 = 1 + sin(4*%pi*x / 10);
y2 = x - cos(4*%pi*x / 10) / (4*%pi/10);

subplot(2,1,1);
plot(x,y1,'k-');
xstring(6,1.4,'$\leftarrow f \left( x \right) = 1 + \sin \left( \frac{x \cdot 4 \cdot \pi}{10} \right)$');

subplot(2,1,2);
plot(x,y2,'k-');
xstring(6,3,'$\leftarrow \frac{df \left( x \right)}{dx} = x - \frac{\cos \left( \frac{x \cdot 4 \cdot \pi}{10} \right)}{\frac{4 \cdot \pi}{10}}$');

// DEMO END
