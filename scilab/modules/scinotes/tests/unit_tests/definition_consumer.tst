// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Definition navigation to another file (consumer side).
//
// With definition_library.tst open but not executed:
// 1. Both calls must use the macro color.
// 2. Hover must show the signature and definition_library.tst as source.
// 3. Ctrl+click must open definition_library.tst at the correct function.
// 4. Type "monacoCrossFile" and request completion: both library macros must
//    be proposed once with definition_library.tst as their source.
//
// After executing definition_library.tst:
// 5. Hold Ctrl over each call: Monaco must show that it is navigable.
// 6. Color, hover and navigation must remain equivalent to the static case.
// 7. Repeat with the library tab already open and after closing that tab.

1+1

squareResult = monacoCrossFileSquare(5);
cubeResult = monacoCrossFileCube(3);
