// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
// Copyright (C) 2016 - Scilab Enteprises - Piere-Aime AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13918 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/13918
//
// <-- Short Description -->
// invalid operation on hypermatrix must call overload functions

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//error msg
msg1 = _("Undefined operation for the given operands.\n");
msg2 = _("check or define function %s for overloading.\n");

//define hypermatrix
a3 = rand(2, 2, 2);
a2 = rand(2, 2);


//addition
//sparse + double
assert_checkerror("sparse(a2) + a3", msprintf(msg1 + "%s", msprintf(msg2, "%sp_a_s")));
assert_checkerror("a3 + sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_a_sp")));

// divide
//double / double
assert_checkerror("a2 / a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_r_s")));
assert_checkerror("a3 / a2", msprintf(msg1 + "%s", msprintf(msg2, "%s_r_s")));

//sparse / double
assert_checkerror("sparse(a2) / a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_r_s")));
assert_checkerror("a3 / sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_r_s")));

// dotdivide
//double ./ sparse
assert_checkerror("a3 ./ sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_d_sp")));

//sparse ./ double
assert_checkerror("sparse(a2) ./ a3", msprintf(msg1 + "%s", msprintf(msg2, "%sp_d_s")));

//dotmult
// sparse .* double
assert_checkerror("sparse(a2) .* a3", msprintf(msg1 + "%s", msprintf(msg2, "%sp_x_s")));

//double .* sparse
assert_checkerror("a3 .* sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_x_sp")));

// ldivide
//double \ double
assert_checkerror("a3 \ a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_l_s")));
assert_checkerror("a2 \ a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_l_s")));
assert_checkerror("a3 \ a2", msprintf(msg1 + "%s", msprintf(msg2, "%s_l_s")));

//multiplication
//double * double
assert_checkerror("a3 * a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_m_s")));
assert_checkerror("a3 * a2", msprintf(msg1 + "%s", msprintf(msg2, "%s_m_s")));
assert_checkerror("a2 * a3", msprintf(msg1 + "%s", msprintf(msg2, "%s_m_s")));

//sparse * double
assert_checkerror("sparse(a2) * a3", msprintf(msg1 + "%s", msprintf(msg2, "%sp_m_s")));

//double * sparse
assert_checkerror("a3 * sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_m_sp")));

// substraction
//double - sparse
assert_checkerror("a3 - sparse(a2)", msprintf(msg1 + "%s", msprintf(msg2, "%s_s_sp")));

//sparse - double
assert_checkerror("sparse(a2) - a3", msprintf(msg1 + "%s", msprintf(msg2, "%sp_s_s")));
