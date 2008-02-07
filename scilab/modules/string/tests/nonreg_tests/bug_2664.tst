// <-- Non-regression test for bug 2664 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2664
//
// <-- Short Description -->
// 
//  strsubst() cannot be called in regexp mode
//
// =============================================================================
// Author : Pierre MARECHAL
// Scilab Project
// 29 janvier 2008
// =============================================================================

strsubst('2' ,'/2(]*)?$\1/' ,'0','r')
