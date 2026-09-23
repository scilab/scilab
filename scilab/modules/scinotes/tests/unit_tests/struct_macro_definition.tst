// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Macros stored in runtime structure fields.
//
// Execute this file, then check:
// 1. callback and nestedCallback use the macro color, with or without ().
// 2. Ctrl+click callback and nestedCallback opens the real factor source,
//    never a /tmp/LOAD_INTO_SCILAB-*.sce file.
// 3. Hover callback and nestedCallback shows the factor documentation.
// 4. value remains a field and Ctrl+click on it does nothing.

handlers = struct();
handlers.callback = factor;
handlers.value = 42;
handlers.nested = struct("nestedCallback", factor);

handlers.callback(12);
handlers.value;
handlers.nested.nestedCallback(12);
