// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Signature help and active argument detection.
//
// Do not execute the file. Move the caret through every call and check:
// 1. The active signature follows the innermost call.
// 2. The highlighted argument changes immediately after each comma.
// 3. Commas inside matrices and strings do not increment the parent argument.

nested = matrix(zeros(2, 3), 3, 2);

multiline = matrix(..
    ones(2, 3), ..
    3, ..
    2);

fromMatrix = matrix([1, 2; 3, 4], 2, 2);
withCommas = strsubst("a,b,c", ",", ";");

// 4. Press Ctrl+Space immediately after ( or ,: signature help must open.
// 5. Press Ctrl+Space after beginning an identifier: completion must open.
// 6. Temporarily type "matrix([1, 2," and verify that the lexical fallback
//    still reports the first matrix argument.
// 7. Temporarily type "matrix(" and verify fallback on an incomplete AST.
// 8. In "matrix(zeros(2, 3),| 3, 2)", the caret marked | must select the
//    second matrix argument, not a third zeros argument.
// 9. Before executing this file, invoke signature help in both calls to
//    signature below. The AST prototype must be available whether the call is
//    located before or after the declaration.

beforeDeclaration = signature(1, 2, 3);

function [status, output] = signature(A, B, C)
    output = A + C + B;
endfunction

afterDeclaration = signature(1, 2, 3);

// 10. Execute only the deff line, then invoke signature help in the following
//     call. The runtime macro prototype must be available without installed
//     documentation.
deff("runtimeResult=runtimeSignature(left,right)", "runtimeResult=left+right");
runtimeValue = runtimeSignature(1, 2);

// 11. For matrix and zeros above, verify that the active parameter includes
//     its documentation when the installed help page documents that argument.
