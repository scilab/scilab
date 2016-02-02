//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

// <-- CLI SHELL MODE -->

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
// M .** M <-- This does not exists in Scilab ... Wired ...

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
