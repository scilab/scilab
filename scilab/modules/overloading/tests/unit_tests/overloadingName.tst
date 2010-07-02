//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- JVM NOT MANDATORY -->

// <-- ENGLISH IMPOSED -->

//
// This test is to check how overloading names are
// built then called.
// use mlist to define unknown type function.
//
// This test is base on the diary output.
//

overloadingNames = [
    "%unknown_p" // Display
    "%unknown_e" // Extraction
    "%unknown_i_unknown" // Insertion
    "%unknown_t" // transpose
    "%unknown_0"
    "%unknown_a_unknown" // addition
    "%unknown_s_unknown" // subtraction
    "%unknown_m_unknown" // mutiplication
    "%unknown_x_unknown"
    "%unknown_k_unknown"
    "%unknown_u_unknown"
    "%unknown_r_unknown" // division
    "%unknown_d_unknown"
    "%unknown_y_unknown"
    "%unknown_v_unknown"
    "%unknown_l_unknown" // right left division
    "%unknown_q_unknown"
    "%unknown_z_unknown"
    "%unknown_w_unknown"
    "%unknown_p_unknown" // power
    "%unknown_j_unknown"
    "%unknown_c_unknown" // column concat
    "%unknown_f_unknown" // row concat
    "%unknown_b_unknown" // implicit list
    "%unknown_o_unknown" // equal
    "%unknown_n_unknown" // different
    "%unknown_g_unknown" // or
    "%unknown_h_unknown" // and
    "%unknown_1_unknown" // <
    "%unknown_2_unknown" // >
    "%unknown_3_unknown" // <=
    "%unknown_4_unknown" // >=
    "%unknown_5" // not
];


// Generate all overloading functions...

for name = overloadingNames
  execstr("function r="+name+"(varargin); " + ...
         "disp(""Calling "+name+"...""); " + ...
         "r = []; " + ...
         "endfunction");
end


M = mlist("unknown");

if typeof(M) <> "unknown" then pause,end

M

M(1)
M(1,2)

N = M
N(2) = M

M'
M.'

M + M

M - M

M * M
M .* M
M *. M
M .*. M

M / M
M ./ M
M /. M
M ./. M

M \ M
M .\ M
M \. M
M .\. M

M ^ M
M ** M
M .^ M
// M .** M <-- This do not exists in scilab ... Wired ...

M:M

M == M

M <> M
M ~= M
M @= M

M < M
M > M

M <= M
M >= M

M | M
M & M

~M
@M

// Clear all added functions
for name = overloadingNames
  execstr("clear "+name);
end
