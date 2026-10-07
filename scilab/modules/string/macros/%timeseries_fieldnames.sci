// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
// For more information, see the COPYING file which you should have received
// along with this program.

function s = %timeseries_fieldnames(a)
    s = [a.props.VariableNames, "Properties", "Variables"];
endfunction
