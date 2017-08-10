// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15057 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15057
//
// <-- Short Description -->
// update of data of matplot does not take care of dimension >2

a = uint8(zeros(480,640,3));
a(:,:,1) = uint8(255);
a(:,:,2) = uint8(180);
a(:,:,3) = uint8(60);
Matplot(a);
e = gce();

//change `data` values to be sure data are updated
e.data = a + 1;
assert_checkequal(e.data, a+1);
