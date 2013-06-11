// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("simple.dem.sce");

// DEMO START

f="<mrow><mi>&#x3b8;</mi><mo lspace=""0.278em"" rspace=""0.278em"">=</m"+...
"o><mi>sin</mi><mspace width=""0.167em""/><mi>&#x3b1;</mi><mo"+...
" lspace=""0.222em"" rspace=""0.222em"">&#xd7;</mo><mi>cos</mi><mspace width=""0.167em""/><mi>&#x3b2;</mi></mrow>";

a="<mi>&#x3b1;</mi>";
b="<mi>&#x3b2;</mi>";
t="<mi>&#x3b8;</mi>";

xtitle(f,a,b,t);
plot3d;

// DEMO END
