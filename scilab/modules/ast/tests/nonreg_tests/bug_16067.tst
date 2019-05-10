// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 16067 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16067
//
// <-- Short Description -->
//The display of matrices of signed integers is misaligned (int8 int16 int32 int64)(regression of 6.0.0 vs 5.5.2)

for i=1:4
    int8(2^7*grand(3,4,'unf',-1,1))
    int16(2^15*grand(3,4,'unf',-1,1))
    int32(2^31*grand(3,4,'unf',-1,1))
    int64(2^63*grand(3,4,'unf',-1,1))

    uint8(2^8*grand(3,4,'unf',0,1))
    uint16(2^16*grand(3,4,'unf',0,1))
    uint32(2^32*grand(3,4,'unf',0,1))
    uint64(2^64*grand(3,4,'unf',0,1))
end
