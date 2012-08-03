// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10416 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10416
//
// <-- Short Description -->
// xmltoformat did not return at the root directory in case of error.

// to test here, we can check with toolbox_skeleton and add a error in some xml files

cur_ref_dir = pwd();

edit SCI/contrib/toolbox_skeleton/help/en_US/c_multiplybypi.xml

// add a error in c_multiplybypi.xml 

exec builder.sce;

cur_dir = pwd();

assert_check(cur_dir, cur_ref_dir);
