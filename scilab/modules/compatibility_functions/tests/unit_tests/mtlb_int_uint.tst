// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

imin0 = {-128, -2^15, -2^31, -(int64(2)^63), 0, 0, 0, 0}
imax0 = {127, 2^15-1, 2^31-1, -(int64(2)^63)-1, 2^8-1, 2^16-1, 2^32-1, uint64(0)-1}
t = ["int8" "int16" "int32" "int64" "uint8" "uint16" "uint32" "uint64"];

funcprot(0)
for i = 1:8
    execstr("f="+t(i));
    execstr("fm=mtlb_"+t(i));
    imin = imin0{i};
    imax = imax0{i};
    assert_checkequal(fm(%nan), f(0));
    assert_checkequal(fm(-%inf), f(imin));
    assert_checkequal(fm(imin), f(imin));
    if i==4
        assert_checkequal(fm(-(2^63)), f(imin));
        assert_checkequal(fm(-(2^63)-1), f(imin));
        assert_checkequal(fm(-(2^63)-2000), f(imin));
    elseif i==8
        assert_checkequal(fm(-(2^64)), f(imin));
        assert_checkequal(fm(-(2^64)-1), f(imin));
        assert_checkequal(fm(-(2^64)-4000), f(imin));
    else
        assert_checkequal(fm(imin-1), f(imin));
        assert_checkequal(fm(imin-2), f(imin));
    end
    assert_checkequal(fm(imin+1), f(imin+1));
    assert_checkequal(fm(imin+2), f(imin+2));

    if i<5
        assert_checkequal(fm(-8.2), f(-8));
        assert_checkequal(fm(-8.5), f(-9));
        assert_checkequal(fm(-8.6), f(-9));
    end
    assert_checkequal(fm(10.2), f(10));
    assert_checkequal(fm(10.5), f(11));
    assert_checkequal(fm(10.6), f(11));

    assert_checkequal(fm(imax-2), f(imax-2));
    assert_checkequal(fm(imax-1), f(imax-1));
    assert_checkequal(fm(imax), f(imax));

    if i==4
        assert_checkequal(fm(2^63), f(imax));
    elseif i==8
        assert_checkequal(fm(2^64), f(imax));
    else
        assert_checkequal(fm(imax+1), f(imax));
        assert_checkequal(fm(imax+10), f(imax));
    end

    assert_checkequal(fm(%inf), f(imax));
end
