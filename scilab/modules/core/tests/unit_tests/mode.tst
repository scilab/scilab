// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

function a = test_mode_foo(level_mode)
  mode(level_mode);
  a = mode();
endfunction

ierr = execstr('mode(7.2);','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mode(''ffo'');','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mode([1 2]);','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mode(%t);','errcatch');
if ierr <> 999 then pause,end

ref = -1;
r = test_mode_foo(ref);
if r <> ref then pause,end

ref = 0;
r = test_mode_foo(ref);
if r <> ref then pause,end

ref = 500;
r = test_mode_foo(ref);
if r <> ref then pause,end

ref = 2;
mode(ref);
if ref <> mode() then pause,end


exec("SCI/modules/core/tests/unit_tests/script_exec.sce", -1);
exec("SCI/modules/core/tests/unit_tests/script_exec.sce", 0);
exec("SCI/modules/core/tests/unit_tests/script_exec.sce", 1);
exec("SCI/modules/core/tests/unit_tests/script_exec.sce", 2);
exec("SCI/modules/core/tests/unit_tests/script_exec.sce", 3);

m=-1;mode(2);exec("SCI/modules/core/tests/unit_tests/script_mode.sce");
m=0;mode(2);exec("SCI/modules/core/tests/unit_tests/script_mode.sce");
m=1;mode(2);exec("SCI/modules/core/tests/unit_tests/script_mode.sce");
m=2;mode(2);exec("SCI/modules/core/tests/unit_tests/script_mode.sce");
m=3;mode(2);exec("SCI/modules/core/tests/unit_tests/script_mode.sce");
