// <-- Non-regression test for bug 656 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=656
//
// <-- Short Description -->
//    Not the same behaviour between Linux mode and Windows mode :
//
//    isdir(getenv('SCI'))   
//    isdir(getenv('SCI')+"/")   
//
//    True for Linux, the first one is True for Windows and False 
//    for the second one


// bug656
// 

mode (-1)
clear
affich_result( isdir(getenv('SCI')) == isdir(getenv('SCI')+"/") , 656)
clear


