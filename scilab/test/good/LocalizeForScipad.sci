function locstr = LocalizeForScipad(strtag)
    // Make use of msgcat from Scipad to localize as locstr the input
    // string strtag
    // This function is intended to be used only from Scipad .sci or
    // .sce ancillaries, therefore no argument checks nor checks that
    // Tcl is installed nor checks that Scipad interpreter exists, etc.
    // Author: François Vogel, 2006
    // Note: this makes use of request 319 implementation (TCL_EvalStr
    //       returns the result of the evaluation)
    locstr = TCL_EvalStr("mc """+strtag+"""","scipad")
endfunction
