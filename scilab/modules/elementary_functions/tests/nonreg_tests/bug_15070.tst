// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15070 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15070
//
// <-- Short Description -->
//    bitset() failed when the bit position was specified as an encoded integer

for v_enc = [11 12 14]           // value encoding
    j = 0;
    for bp_enc = [11 12 14]      // bit position encoding
        value0 = iconvert(0, v_enc);
        ind = iconvert((1:8)+(v_enc-11)*8, bp_enc);
        for bp = ind
          value = bitset(value0, bp);
          assert_checkequal(value, value0 + 2^iconvert(bp-1, v_enc));
          value0 = value;
        end
    end
end
