// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8684 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8684
//
// <-- Short Description -->
// 'wfir' function produced an error message when used interactively.

// 1. type wfir() in Scilab Console
// 2. choose low pass filter type
// 3. set cut-off frequency to e.g. 0.03
// 4. set filter length to e.g. 64
// 5. choose Chebyshev window type
// ==> Check that a message box is opened and no error message displayed