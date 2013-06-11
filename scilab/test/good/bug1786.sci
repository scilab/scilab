// Non-regression test file for bug 1786
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

mode(-1);
clear

hf = scf (1);
ha = gca ();
hlt = ha.title;

hlt.text = "Titre/Title";

test1 = (hlt.auto_rotation == "on");
test2 = (hlt.auto_position == "on");

hlt.font_angle = 180;
test3 = (hlt.auto_rotation == "off");

hlt.position = [0.5,0.5];
test4 = (hlt.auto_position == "off");

if( test1 & test2 & test3 & test4 ) then
    affich_result(%T,1786);
else
    affich_result(%F,1786);
end

clear
