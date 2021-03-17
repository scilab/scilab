// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16511 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16512
//
// <-- Short Description -->
// 1 ./ uint8(0) crashes Scilab

for cast = list(int8, int16, int32, int64, uint8, uint16, uint32, uint64)
    assert_checkequal(1 ./ cast(0),  1 / cast(0));
    assert_checkequal(-1 ./ cast(0),  -1 / cast(0));
    assert_checkequal(1 ./ -cast(0),  1 / -cast(0));
    assert_checkequal(-1 ./ -cast(0),  -1 / -cast(0));
    assert_checkequal(cast(1)  ./ 0, cast(1) / 0);
    assert_checkequal(-cast(1)  ./ 0, -cast(1) / 0);
    assert_checkequal(cast(1)  ./ -0, cast(1) / -0);
    assert_checkequal(-cast(1)  ./ -0, -cast(1) / -0);
end
