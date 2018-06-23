// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 15617 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15617
//
// <-- Short Description -->
// * evstr(list(..)): the number and types of the list components were
//   not checked
// * When some given expression can't be evaluated, the error message
//   was misleading

a = 1;
clear b
msg = _("%s: Argument #%d: %d-element list expected.\n")
assert_checkerror("evstr(list([''b'',''a'']))", msprintf(msg, "evstr", 1, 2));

msg = _("%s: Argument #%d: Both list components must be of text type.\n")
assert_checkerror("evstr(list(%t,""a""))", msprintf(msg, "evstr", 1));
assert_checkerror("evstr(list(""a+1"",%f))", msprintf(msg, "evstr", 1));

msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n");
submsg = msprintf(_("Undefined variable: %s\n"), "b");
assert_checkerror("evstr([""a+1"" ""b-1""])", msprintf(msg, "evstr", 1, submsg));

msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n");
submsg = msprintf(_("Undefined variable: %s\n"), "%");
assert_checkerror("evstr([""sin(4)"" ""a+%(1)""])", msprintf(msg, "evstr", 1, submsg));

msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n");
submsg = msprintf(_("Undefined variable: %s\n"), "b");
assert_checkerror("evstr(list([""b-1"" ""a+1""],""2+3""))", msprintf(msg, "evstr", 1, submsg));

msg = _("%s: Argument #1(2): The expression #%d = ""%s"" can''t be evaluated (%s).\n")
submsg = msprintf(_("Undefined variable: %s\n"), "b");
assert_checkerror("evstr(list([""sin(4)"" ""a+%(1)""],""b""))", msprintf(msg, "evstr", 1, "b", submsg));

msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n");
submsg = msprintf(_("Invalid index.\n"));
assert_checkerror("evstr(list([""sin(4)"" ""a+%(3)""],[""2"" ""2*a""]))", msprintf(msg, "evstr", 1, submsg));

msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n");
submsg = msprintf(_("Function not defined for given argument type(s),\n"));
submsg = submsg + msprintf(_("  check arguments or define function %s for overloading.\n"), "%c_cos");
assert_checkerror("evstr(""cos(""""s"""")"")", msprintf(msg, "evstr", 1, submsg));
