// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITOE - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10841 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10841
//
// <-- Short Description -->
//    check default value of sliderStep property

singleValue = %pi/100;
origValues = [%pi/100, %pi/10];
defaultValues = [%pi/100, %pi/10];
u=uicontrol("style","slider","position",[10 10 200,30],"max",%pi);
u.sliderStep = origValues;
assert_checkequal(u.sliderStep, origValues);
u.sliderStep = singleValue;
assert_checkequal(u.sliderStep, defaultValues);
