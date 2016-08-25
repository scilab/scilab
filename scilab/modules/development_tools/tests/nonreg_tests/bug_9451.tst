// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9451 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9451
//
// <-- Short Description -->
// test_run() output was not clear enough when several modules are tested
//

test_run();

//Expected output:
//----------------
// Module  001/076 - [action_binding] :
//
//   001/001 - [action_binding] bug_6743..........................skipped: interactive test
//
// Module  002/076 - [api_scilab] :
//
//   001/051 - [api_scilab] booleanExample........................failed: Slave Scilab exited with error code 1
//   002/051 - [api_scilab] booleanSparseExample..................failed: Slave Scilab exited with error code 1
//   003/051 - [api_scilab] boolean_writing_api...................failed: Slave Scilab exited with error code 1
//   004/051 - [api_scilab] calloverloadfunction_api..............failed: Slave Scilab exited with error code 1
//   005/051 - [api_scilab] check_matrix_dimension................
//...
//
//Instead of:
//-----------
//   001/076 - [action_binding] :
//
//   001/001 - [action_binding] bug_6743..........................skipped: interactive test
//
//   002/076 - [api_scilab] :
//
//   001/051 - [api_scilab] booleanExample........................failed: Slave Scilab exited with error code 1
//   002/051 - [api_scilab] booleanSparseExample..................failed: Slave Scilab exited with error code 1
//   003/051 - [api_scilab] boolean_writing_api...................failed: Slave Scilab exited with error code 1
//   004/051 - [api_scilab] calloverloadfunction_api..............failed: Slave Scilab exited with error code 1
//   005/051 - [api_scilab] check_matrix_dimension................
//...
