// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14629 -->
// <-- Non-regression test for bug 14680 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14629
// http://bugzilla.scilab.org/14680
//
// <-- Short Description -->
// "<" characters of the expression could truncate the rendering of the expressionn in the icon
//  of EXPRESSION blocks

// * In the "User-Defined Functions" palette, drag and drop an EXPRESSION block in the diagram
// * Double-click on its icon
// * 14629: Input (u1>scm) * (u1<scp) as the expression
//   Press "OK": the expression must be fully rendered in the icon, without being truncated
// * 14680: In the expression, add some "<", "<>" or "<=" sequences
//   Press "OK": the updated rendering must show all the expression
