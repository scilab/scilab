// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function s = %table_fieldnames(a)
    s = [a.props.VariableNames, "Properties", "Variables", "Row"];
endfunction
