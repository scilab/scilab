// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3923 -->
//

// <-- Short Description -->
// diary() function produces bad files when it try to record interactive commands : it works when diary is used inside a
// script file.

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3923
//


// copy paste these lines
diary(TMPDIR+"/bug_diary.txt");
A = rand(2,2)
diary(0);
mgetl(TMPDIR+"/bug_diary.txt")


// you should have :

-->diary(TMPDIR+"/bug_diary.txt");
 
-->A = rand(2,2)
 A  =
 
    0.2113249    0.0002211  
    0.7560439    0.3303271  
 
-->diary(0);
 
-->mgetl(TMPDIR+"/bug_diary.txt")
 ans  =
 
!                              !
!                              !
!-->A = rand(2,2)              !
!                              !
! A  =                         !
!                              !
!                              !
!                              !
!    0.2113249    0.0002211    !
!                              !
!    0.7560439    0.3303271    !
!                              !
!                              !
!                              !
!-->diary(0);                  !
 