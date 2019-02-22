// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15974 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15974
//
// <-- Short Description -->
// msprintf(*, %nan) doesn't return Nan

mprintf("%f\n",%inf)
mprintf("%f\n",-%inf)
mprintf("%f\n",%nan)

mfprintf(6,"%f\n",%inf)
mfprintf(6,"%f\n",-%inf)
mfprintf(6,"%f\n",%nan)

infStr = msprintf("%f",%inf);
minusInfStr = msprintf("%f",-%inf);
nanStr = msprintf("%f",%nan);

assert_checkequal(infStr,"Inf");
assert_checkequal(minusInfStr,"-Inf");
assert_checkequal(nanStr,"Nan");
