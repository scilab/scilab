// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI-Group - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

s = slint("SCI/modules/slint/tests/nonreg_tests/bug_14946.sci", %f);
assert_checkfalse(isfield(s.info, "00003.Uninitialized"));
