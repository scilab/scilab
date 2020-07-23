// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for clock() function
// =============================================================================


[t, tz, Dst] = clock();
assert_checkequal(size(t), [1 6]);
assert_checktrue(and(t(1:5) ==int(t(1:5))));
assert_checktrue(t(1) >= 2020);
assert_checktrue(t(2) >= 1 & t(2) <= 12);
assert_checktrue(t(3) >= 1 & t(3) <= 31);
assert_checktrue(t(4) >= 0 & t(4) < 24);
assert_checktrue(t(5) >= 0 & t(5) < 60);
assert_checktrue(t(6) >= 0 & t(6) < 60);

// Time Zone
assert_checktrue(tz > -13 & tz < 13);
assert_checktrue(modulo(tz,0.25)==0);
if getos()=="Windows" then
    r = powershell("Get-TimeZone");
    i = grep(r,"(UTC")
    [s,e,m,f] = regexp(r(i), "/\(UTC(...)\:(..)\)/")
    tzRef = evstr(f);
    assert_checkequal([int(tz) round((tz-int(tz))*60)], tzRef);
end

// Daylight Saving Time
assert_checktrue(or(Dst==[0 1]));
