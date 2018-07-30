// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14708 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14708
//
// <-- Short Description -->
// polynomials: Operators + - / ./ ^ .^ no longer simplify null high orders coefficients from inputs or of the result


p=poly([1 +2],'s','coeff');
q=poly([1 -2],'s','coeff');
assert_checkequal(p+q,poly(2,'s','coeff'));

zerop=poly(0,'s','coeff');
assert_checkequal(%s/%inf,zerop);
assert_checkequal(%s./%inf,zerop);
assert_checkequal(%s./[%inf %inf],[zerop zerop]);
assert_checkequal([%s %s]/%inf,[zerop zerop]);
assert_checkequal([%s %s]./%inf,[zerop zerop]);
assert_checkequal([%s %s]./[%inf %inf],[zerop zerop]);

assert_checkequal(%s.*0,zerop);
assert_checkequal(%s.*[0 0],[zerop zerop]);
assert_checkequal([%s %s].*0,[zerop zerop]);
assert_checkequal([%s %s].*[0 0],[zerop zerop]);

assert_checkequal(%s.*zerop,zerop);
assert_checkequal(%s.*[zerop zerop],[zerop zerop]);
assert_checkequal([%s %s].*zerop,[zerop zerop]);
assert_checkequal([%s %s].*[zerop zerop],[zerop zerop]);