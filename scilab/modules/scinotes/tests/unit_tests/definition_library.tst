// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Runtime definitions located in another file (library side).
//
// 1. Open this file and definition_consumer.tst.
// 2. First keep this file unexecuted and perform the static cross-document
//    checks described in the consumer.
// 3. Execute this file, then return to the consumer for the runtime test.
// 4. Keep both files open during the complete test.

function result = monacoCrossFileSquare(value)
    result = value * value;
endfunction

function result = monacoCrossFileCube(value)
    result = value * monacoCrossFileSquare(value);
endfunction
