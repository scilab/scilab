// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2008 - INRIA Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
exec ('SCI/modules/javasci/tests/unit_tests/test_java.sci',-1);

java_file_to_test = 'Disp.java';

if test_java(java_file_to_test) <> %T then pause,end;

// ====================================================================
