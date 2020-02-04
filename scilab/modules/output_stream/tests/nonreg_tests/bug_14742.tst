// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14742 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14742
//
// <-- ENGLISH IMPOSED -->

// <-- Short Description -->
// Wrong display of Inf or Nan
//

assert_checkequal(" Nan", msprintf('%4.1f',%nan));
assert_checkequal(" Inf", msprintf('%4.1f',%inf));
assert_checkequal("-Inf", msprintf('%4.1f',-%inf));

assert_checkequal("Nan ", msprintf('%-4.1f',%nan));
assert_checkequal("Inf ", msprintf('%-4.1f',%inf));
assert_checkequal("-Inf", msprintf('%-4.1f',-%inf));

assert_checkequal("Nan", msprintf('%1.1f',%nan));
assert_checkequal("Inf", msprintf('%1.1f',%inf));
assert_checkequal("-Inf", msprintf('%1.1f',-%inf));

assert_checkequal("Nan", msprintf('%-1.1f',%nan));
assert_checkequal("Inf", msprintf('%-1.1f',%inf));
assert_checkequal("-Inf", msprintf('%-1.1f',-%inf));

F="%10.3f";
assert_checkequal("     3.000       Nan     5.000", msprintf(F+F+F, [3,%nan,5]));
assert_checkequal("     3.000       Inf     5.000", msprintf(F+F+F, [3,%inf,5]));
assert_checkequal("     3.000      -Inf     5.000", msprintf(F+F+F, [3,-%inf,5]));

F="%-10.3f";
assert_checkequal("3.000     Nan       5.000     ", msprintf(F+F+F, [3,%nan,5]));
assert_checkequal("3.000     Inf       5.000     ", msprintf(F+F+F, [3,%inf,5]));
assert_checkequal("3.000     -Inf      5.000     ", msprintf(F+F+F, [3,-%inf,5]));


assert_checkequal(["a = Inf";"b = Inf"], msprintf("a = %f\nb = %f", %inf, %inf));
assert_checkequal(["a = Nan";"b = Nan"], msprintf("a = %f\nb = %f", %nan, %nan));

assert_checkequal(["a = Inf";"b = Inf"], msprintf("a = %f\nb = %f\n", %inf, %inf));
assert_checkequal(["a = Nan";"b = Nan"], msprintf("a = %f\nb = %f\n", %nan, %nan));

assert_checkequal("Inffoo", msprintf("%ffoo\n", %inf));
assert_checkequal("Nanfoo", msprintf("%ffoo\n", %nan));

assert_checkequal("fooInf", msprintf("foo%f\n", %inf));
assert_checkequal("fooNan", msprintf("foo%f\n", %nan));

assert_checkequal("fooInfbar", msprintf("foo%fbar\n", %inf));
assert_checkequal("fooNanbar", msprintf("foo%fbar\n", %nan));

