// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2154 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2154
//
// <-- Short Description -->
// color_flag and cdata_mapping Fac3d propeties cannot be set

t=[0:0.3:2*%pi]'; z=sin(t)*cos(t');
[xx,yy,zz]=genfac3d(t,t,z);
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
e=gce();
e.color_flag // valeur 2 ok
errorNum = execstr("e.color_flag=2", "errcatch"); //message d'erreur : bug
if (errorNum <> 0) then pause; end;

e.cdata_mapping // valeur direct=ok
errorNum = execstr("e.cdata_mapping=''direct''", "errcatch"); //message d'erreur : bug
if (errorNum <> 0) then pause; end;


