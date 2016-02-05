//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a=erfi(1.234 + 0.5678*%i);
assert_checkalmostequal(a, 1.081032284405+%i*1.926775520841);


assert_checkerror("erfi(''foo'')",[],999);
assert_checkerror("erfi(sparse([1,2;4,5;3,10],[1,2,3]))",[],999);
