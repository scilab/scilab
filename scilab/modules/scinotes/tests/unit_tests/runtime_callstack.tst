// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Execution error baseline (not a Monaco/LSP acceptance criterion).
//
// Execute this file and inspect the call stack. SciNotes may execute through a
// /tmp/LOAD_INTO_SCILAB-*.sce file on Linux; this behavior also exists without
// Monaco and is therefore recorded only as a baseline, not as a failure here.
// Verify that Monaco remains usable and that the source document is unchanged
// after the execution error.
function value = undocumentedHover(value)
    value = value + 1;
endfunction

function value = monacoCallstackInner(input)
    value = input + missingMonacoCallstackVariable;
end

function value = monacoCallstackOuter(input)
    value = monacoCallstackInner(input);
end

monacoCallstackOuter(1);
