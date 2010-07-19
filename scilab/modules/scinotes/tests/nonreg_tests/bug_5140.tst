// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5140 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5140
//
// <-- Short Description -->
// "Load into Scilab" from the text editor on a big file is taking forever.
// All are executed line by line.
// we should save into a file and ask Scilab to load it. It will be much faster.

// launch editor
// paste some line in a tab 
// try "Load into Scilab

// paste theses lines:
1+1
2
A= 3
disp(A)

// do "Load into Scilab" 
// result display in console should be:


//-->editor
  
-->1+1
 ans  =
 
    2.  
-->2
 ans  =
 
    2.  
-->A= 3
 A  =
 
    3.  
-->disp(A)
 
    3.  


// creates a file with more 100 lines or more than 80 * 100 characters
// do "Load into Scilab" 
// a message box should be opened , try 'yes' and 'no'
