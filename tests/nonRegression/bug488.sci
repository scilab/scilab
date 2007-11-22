// <-- Non-regression test for bug 488 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=488
//
// <-- Short Description -->
//    Bug Report Id: 122062003795731
//     seems to have popped up in 2.7.2 - did not happen in 2.7.1
//
//
//
//    On Scilab 2.7.2 with " Help menu or Editor Menu " function
//    The Error Messages are:
//       !--error   999
//    TK_EvalStr, Can't find a usable init.tcl in the following directories:
//        {C:/Program Files/Scilab-2.7.2\tcl\tcl8.3} {C:/Program Files/Scilab-2.7.2\tcl\tcl8.3} {C:/Program Files/Scilab-2.7.2
//    /tcl/tcl8.3} {C:/Program Files/Scilab-2.7.2/lib/tcl8.3}
//
//
//
//    This probably means that Tcl wasn't installed properly.
//     at line 1
//    at line      13 of function tcltk_help               called by :
//    line    22 of function run_help                 called by :
//    line    46 of function browsehelp               called by :
//    line    10 of function help                     called by :
// ...


// bug488
// 489, 491 492 496 duplicate

mode (-1)
clear

// rechercher l'existance du répertoire tcl sous Scilab

correct=%F
tcl_dir=getenv('SCI')+"/modules/tclsci/tcl"
correct=isdir(tcl_dir)
if correct & MSDOS then
   // si oui rechercher l'existance du fichier init.tcl sous un repertoire de tcl
   tcl_list = basename(listfiles(listfiles(tcl_dir+"/tcl*")+"/init.*")) 
   correct = (grep ( tcl_list , "init") <> []) | ( grep ( tcl_list , "INIT") <> [])
end // du if

affich_result(correct, 488)

clear

// Error message :
//  --> !--error   999
// TK_EvalStr, Can't find a usable init.tcl in the following directories:
// {C:/PROGRAM FILES/SCILAB-2.7.2\tcl\tcl8.3}{...
// 
// Tcl wasn't installed properly
