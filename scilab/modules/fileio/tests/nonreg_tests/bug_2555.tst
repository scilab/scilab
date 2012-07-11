// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1526 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1526
//
// <-- Short Description -->
//   An ascii file (created 'by hand' or a fprintfMat primitive with Scilab/Windows)
//   using windows encoding is read badly on Scilab/Linux with the fscanfMat primitive.
//   This problem seems to come from the bad read of end-line characters.
//   A work-around of this bug, is to use 'dos2unix' on Linux to convert the file.

mat_ref = [ ..
    72. 21. 26. 77. 11. ; ..
    19. 31. 63. 21. 19. ; ..
    54. 36. 40. 11. 56. ; ..
    23. 29. 91. 68. 58. ; ..
    23. 56. 4. 15. 68.  ; ..
    21. 48. 48. 69. 89. ; ..
    88. 33. 26. 84. 50. ; ..
    65. 59. 41. 40. 34. ; ..
    30. 50. 28. 40. 38. ; ..
    93. 43. 12. 87. 92. ];

mat = fscanfMat("SCI/modules/fileio/tests/nonreg_tests/bug_2555.mat")

if or( mat <> mat_ref ) then pause, end
