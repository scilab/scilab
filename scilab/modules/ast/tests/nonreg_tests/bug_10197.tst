// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10197 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10197
//
// <-- Short Description -->
// creating implicit vector of short integers may fail

assert_checktrue(isempty(int8(1):int8(-1)));
assert_checktrue(isempty(int16(1):int16(-1)));
assert_checktrue(isempty(int32(1):int32(-1)));
assert_checktrue(isempty(int64(1):int64(-1)));
assert_checktrue(isempty(int8(120):int8(130)));
assert_checktrue(isempty(int16(32760):int16(32770))); 
assert_checktrue(isempty(int32(-2147483640):int32(2147483650)));
assert_checktrue(isempty(uint8(2):uint8(0)));
assert_checktrue(isempty(uint16(2):uint16(0)));
assert_checktrue(isempty(uint32(2):uint32(0)));
assert_checktrue(isempty(uint64(2):uint64(0)));
assert_checktrue(isempty(uint8(250):uint8(260)));
assert_checktrue(isempty(uint16(65530):uint16(65540))); 
assert_checktrue(isempty(uint32(2):uint32(4294967297)));
