// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11619 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11619
//
// <-- Short Description -->
//titlepage("Text"): the font size is no longer maximized to fit the figure's sizes
//

h1=figure("figure_name", "Big title");
titlepage(["42 is the";" answer"]);

h2=figure("figure_name", "Tiny title");
a1=h2.children;
a1.margins=[0.42,0.42,0.42,0.42];
titlepage(["42 is the";" answer"]);

h3=figure("figure_name", "Two titles");
a1=h3.children;
a1.margins(3)=0.61;
a2=newaxes();
a2.margins(4)=0.39;
sca(a1);
titlepage("Bottom title");
sca(a2);
titlepage("Top title");

// Ensure that the texts correctly fill rectangles.
