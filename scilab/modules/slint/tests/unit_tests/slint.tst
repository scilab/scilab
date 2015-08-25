// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//

a = slint("SCI/modules/slint/tests/unit_tests/files/GlobalKeyword.sci",%f);
assert_checktrue(isfield(a.info, "00001"));
assert_checkequal(size(a.info("00001"), "*"), 1);
assert_checkequal(a.info("00001").loc, [2 3; 2 9]);

a = slint("SCI/modules/slint/tests/unit_tests/files/Redefinition.sci",%f);
assert_checktrue(isfield(a.info, "00002"));
assert_checkequal(size(a.info("00002"), "*"), 3);
assert_checkequal(a.info("00002")(1).loc, [2 3; 2 6]);
assert_checkequal(a.info("00002")(2).loc, [3 3; 3 10]);
assert_checkequal(a.info("00002")(3).loc, [4 3; 4 10]);

a = slint("SCI/modules/slint/tests/unit_tests/files/Variables.sci",%f);
assert_checktrue(isfield(a.info, "00003"));
assert_checkequal(size(a.info("00003"), "*"), 4);
assert_checkequal(a.info("00003")(1).loc, [2 6; 2 7]);
assert_checkequal(a.info("00003")(1).msg, _("Use of non-initialized variable ''a'' may have any side-effects."));
assert_checkequal(a.info("00003")(2).loc, [8 3; 8 4]);
assert_checkequal(a.info("00003")(2).msg, _("Declared variable and might be unused: l."));
assert_checkequal(a.info("00003")(3).loc, [3 2; 3 3]);
assert_checkequal(a.info("00003")(3).msg, _("Declared variable and might be unused: x."));
assert_checkequal(a.info("00003")(4).loc, [8 6; 8 7]);
assert_checkequal(a.info("00003")(4).msg, _("Declared variable and might be unused: r."));

a = slint("SCI/modules/slint/tests/unit_tests/files/Variables.sci",%f);
assert_checktrue(isfield(a.info, "00003"));
assert_checkequal(size(a.info("00003"), "*"), 4);
assert_checkequal(a.info("00003")(1).loc, [2 6; 2 7]);
assert_checkequal(a.info("00003")(1).msg, _("Use of non-initialized variable ''a'' may have any side-effects."));
assert_checkequal(a.info("00003")(2).loc, [8 3; 8 4]);
assert_checkequal(a.info("00003")(2).msg, _("Declared variable and might be unused: l."));
assert_checkequal(a.info("00003")(3).loc, [3 2; 3 3]);
assert_checkequal(a.info("00003")(3).msg, _("Declared variable and might be unused: x."));
assert_checkequal(a.info("00003")(4).loc, [8 6; 8 7]);
assert_checkequal(a.info("00003")(4).msg, _("Declared variable and might be unused: r."));

a = slint("SCI/modules/slint/tests/unit_tests/files/FunctionArgs.sci",%f);
assert_checktrue(isfield(a.info, "00005"));
assert_checkequal(size(a.info("00005"), "*"), 2);
assert_checkequal(a.info("00005")(1).loc, [4 1; 5 12]);
assert_checkequal(a.info("00005")(1).msg, _("Duplicated function arguments: b."));
assert_checkequal(a.info("00005")(2).loc, [7 1; 8 12]);
assert_checkequal(a.info("00005")(2).msg, _("Function arguments used as output values: a."));

a = slint("SCI/modules/slint/tests/unit_tests/files/UselessArg.sci",%f);
assert_checktrue(isfield(a.info, "00006"));
assert_checkequal(size(a.info("00006"), "*"), 1);
assert_checkequal(a.info("00006")(1).loc, [5 29; 5 30]);
assert_checkequal(a.info("00006")(1).msg, _("Function argument might be unused: b."));

a = slint("SCI/modules/slint/tests/unit_tests/files/UselessRet.sci",%f);
assert_checktrue(isfield(a.info, "00007"));
assert_checkequal(size(a.info("00007"), "*"), 1);
assert_checkequal(a.info("00007")(1).loc, [5 11; 5 12]);
assert_checkequal(a.info("00007")(1).msg, _("Function returned value might be unused: y."));

a = slint("SCI/modules/slint/tests/unit_tests/files/SingleInstr.sci",%f);
assert_checktrue(isfield(a.info, "00009"));
assert_checkequal(size(a.info("00009"), "*"), 1);
assert_checkequal(a.info("00009")(1).loc, [2 8; 2 14]);

a = slint("SCI/modules/slint/tests/unit_tests/files/EmptyBlock.sci",%f);
assert_checktrue(isfield(a.info, "00010"));
assert_checkequal(size(a.info("00010"), "*"), 1);
assert_checkequal(a.info("00010")(1).loc, [3 1; 3 1]);

a = slint("SCI/modules/slint/tests/unit_tests/files/MopenMclose.sci",%f);
assert_checktrue(isfield(a.info, "00011"));
assert_checkequal(size(a.info("00011"), "*"), 2);
assert_checkequal(a.info("00011")(1).loc, [1 1; 3 12]);
assert_checkequal(a.info("00011")(1).msg, _("Open files not closed: fd."));
assert_checkequal(a.info("00011")(2).loc, [7 1; 7 14]);

a = slint("SCI/modules/slint/tests/unit_tests/files/McCabe.sci",%f);
assert_checktrue(isfield(a.info, "00012"));
assert_checkequal(size(a.info("00012"), "*"), 1);
assert_checkequal(a.info("00012")(1).loc, [1 1; 91 12]);
assert_checkequal(a.info("00012")(1).msg, _("McCabe''s complexity is 33 and is greater than 30."));

a = slint("SCI/modules/slint/tests/unit_tests/files/Decimal.sci",%f);
assert_checktrue(isfield(a.info, "00013"));
assert_checkequal(size(a.info("00013"), "*"), 3);
assert_checkequal(a.info("00013")(1).loc, [2 5; 2 11]);
assert_checkequal(a.info("00013")(1).msg, _("Decimal numbers musn''t begin by a dot."));
assert_checkequal(a.info("00013")(2).loc, [3 5; 3 11]);
assert_checkequal(a.info("00013")(2).msg, _("Bad decimal exponent: e was expected and d was found."));
assert_checkequal(a.info("00013")(3).loc, [4 5; 4 11]);
assert_checkequal(a.info("00013")(3).msg, _("Bad decimal exponent: e was expected and D was found."));
