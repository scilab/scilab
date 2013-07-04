// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Unit test for DEBUG_SCICOS -->
//
// <-- Short Description -->
// The debug block is called before each real block call
//

global Result;Result=[];

importXcosDiagram('SCI/modules/xcos/tests/nonreg_tests/bug_8480.zcos');
Info=scicos_simulate(scs_m,list());

guis = [];
for i=1:length(scs_m.objs) do
    o = scs_m.objs(i);
    if typeof(o) == "Block" then
        gui=o.gui;
        
        guis = [guis ; gui];
        execstr(gui + "_index = i;");
    end
end

Result_ref = [CLOCK_c_index 4 0;
CLOCK_c_index 4 0;
GENSQR_f_index 4 0;
GENSQR_f_index 4 0;
DOLLAR_f_index 4 0;
DOLLAR_f_index 4 0;
MUX_index 4 0;
MUX_index 4 0;
TRASH_f_index 4 0;
TRASH_f_index 4 0;
CLOCK_c_index 6 0;
CLOCK_c_index 6 0;
GENSQR_f_index 6 0;
GENSQR_f_index 6 0;
DOLLAR_f_index 6 0;
DOLLAR_f_index 6 0;
MUX_index 6 0;
MUX_index 6 0;
TRASH_f_index 6 0;
TRASH_f_index 6 0;
CLOCK_c_index 6 0;
CLOCK_c_index 6 0;
GENSQR_f_index 6 0;
GENSQR_f_index 6 0;
DOLLAR_f_index 6 0;
DOLLAR_f_index 6 0;
MUX_index 6 0;
MUX_index 6 0;
TRASH_f_index 6 0;
TRASH_f_index 6 0;
CLOCK_c_index 1 0;
CLOCK_c_index 1 0;
GENSQR_f_index 1 0;
GENSQR_f_index 1 0;
DOLLAR_f_index 1 0;
DOLLAR_f_index 1 0;
TRASH_f_index 1 0;
TRASH_f_index 1 0;
MUX_index 1 0;
MUX_index 1 0;
CLOCK_c_index 3 0;
CLOCK_c_index 3 0;
CLOCK_c_index 2 0;
CLOCK_c_index 2 0;
GENSQR_f_index 2 0;
GENSQR_f_index 2 0;
DOLLAR_f_index 2 0;
DOLLAR_f_index 2 0;
CLOCK_c_index 1 0.5;
CLOCK_c_index 1 0.5;
GENSQR_f_index 1 0.5;
GENSQR_f_index 1 0.5;
DOLLAR_f_index 1 0.5;
DOLLAR_f_index 1 0.5;
TRASH_f_index 1 0.5;
TRASH_f_index 1 0.5;
MUX_index 1 0.5;
MUX_index 1 0.5;
CLOCK_c_index 3 0.5;
CLOCK_c_index 3 0.5;
CLOCK_c_index 2 0.5;
CLOCK_c_index 2 0.5;
GENSQR_f_index 2 0.5;
GENSQR_f_index 2 0.5;
DOLLAR_f_index 2 0.5;
DOLLAR_f_index 2 0.5;
CLOCK_c_index 5 1;
CLOCK_c_index 5 1;
GENSQR_f_index 5 1;
GENSQR_f_index 5 1;
DOLLAR_f_index 5 1;
DOLLAR_f_index 5 1;
MUX_index 5 1;
MUX_index 5 1;
TRASH_f_index 5 1;
TRASH_f_index 5 1];

if Result <> Result_ref then pause,end
clearglobal Result

