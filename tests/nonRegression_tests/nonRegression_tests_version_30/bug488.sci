// bug488
// 489, 491 492 496 duplicate

mode (-1)
clear

// rechercher l'existance du répertoire tcl sous Scilab

correct=%F
tcl_dir=getenv('SCI')+"/tcl"
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
