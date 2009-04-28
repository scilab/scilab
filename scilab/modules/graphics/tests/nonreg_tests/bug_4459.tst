// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4459 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4459
//
// <-- Short Description -->
// grayplot does not always plot map containing %nan.
// 

//just a map
a=rand(5,5);
//surrounded by %nan
a(:,1)=%nan;a(:,$)=%nan;a(1,:)=%nan;a($,:)=%nan;
//that won't show (empty graph window)
grayplot(1:5,1:5,a);

// check that something appears on the screen. Could be checked by using image processing.

