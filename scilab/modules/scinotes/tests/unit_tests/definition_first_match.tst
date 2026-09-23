// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Deterministic navigation with duplicate definitions.
//
// Keep this file unexecuted:
// 1. Ctrl+click the final monacoDuplicateDefinition call.
// 2. No definition-selection widget must open.
// 3. Navigation must always select the first declaration below.
// 4. Repeat the Ctrl+click several times and verify the result stays stable.

function result = monacoDuplicateDefinition(value)
    result = value + 1;
endfunction

function result = monacoDuplicateDefinition(value)
    result = value + 2;
endfunction

duplicateResult = monacoDuplicateDefinition(40);
