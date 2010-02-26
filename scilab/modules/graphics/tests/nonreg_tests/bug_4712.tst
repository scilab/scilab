// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2009 - INRIA - pierre.lando@scilab.org
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 4712 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4712
//
// <-- Short Description -->
// at src/c/getHandleProperty/set_data_property.c:319
// int monotony = checkMonotony( pvecx, dimvecty ) ;
// instead of
// int monotony = checkMonotony( pvecy, dimvecty ) ;

x=[-10;-8;-7;-5;-3;0;1;8];
y=(-10:10)';
z=cos(x)*sin(y');
plot3d(x,y,z);
e=gce();
e.data.x=e.data.x;

// should not display any warning or error
