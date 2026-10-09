// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 9269 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/9269
//
// <-- Short Description -->
// Specified graphic entities for legend() are drawn in reverse order.
x = 0:10;
y1 = x; y2 = y1+1; y3 = y1+2;

scf(0);clf();
plot(x,y1,"b-v",x,y2,"r--^",x,y3,"k--o");
l=legend("is blue ?","is red ?","is black ?");
assert_checkequal(l.links.foreground,[2 5 1]); // blue red black

scf(0);clf();
plot(x,y1,"b-v"); h1 = gce().children($:-1:1); // first curve (in blue)
plot(x,y2,"r--^"); h2 = gce().children($:-1:1);// second curve (in red)
plot(x,y3,"k--o"); h3 = gce().children($:-1:1);// third curve (in black)
l= legend([h1;h2;h3],"is blue ?","is red ?","is black ?");
assert_checkequal(l.links.foreground,[2 5 1]); // blue red black

delete(l)
l= legend([h1;h2;h3],"is blue ?","is red ?","is black ?");
assert_checkequal(l.links.foreground,[2 5 1]); // blue red black

scf(0);clf();
plot(x,y1,"b-v",x,y2,"r--^"); h1 = gce().children($:-1:1); // first curve (in blue) and second (in red)
plot(x,y3,"k--o"); h3 = gce().children($:-1:1);// third curve (in black)
l=legend([h1;h3],"is blue ?","is red ?","is black ?");
assert_checkequal(l.links.foreground,[2 5 1]); // blue red black


clear;
x = 0:9;
y = rand(10,7);
scf(1);
clf;
plot(x, y);
NameList = ["aze" "rty" "uio" "qsd" "fgh" "jkl" "wxc"];

MyOrder = [4 1 5 3]; // My selection in My order

ha = gca();          // handle on axis
hc = ha.children(1); // handle on compound
he = hc.children;    // handle on polylines
l=legend(he(MyOrder), NameList(MyOrder));
assert_checkequal(l.links.foreground,he(MyOrder)'.foreground);

