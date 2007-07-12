function locstr = LocalizeForScipad(strtag)
// Make use of msgcat from Scipad to localize as locstr the input
// string strtag
// This function is intended to be used only from Scipad .sci or
// .sce ancillaries, therefore no argument checks nor checks that
// Tcl is installed nor checks that Scipad interpreter exists, etc.
// Author: François Vogel, 2006-2007
    TCL_EvalStr("set locstring_scipad [mc """+strtag+"""]","scipad")
    locstr = TCL_GetVar("locstring_scipad","scipad")
endfunction

