// <-- Non-regression test for bug 55 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=55
//
// <-- Short Description -->
//    The sci2exp function gives wrong result with the -%inf 
//    value.
//    sci2exp(-%inf) = -Inf instead of the right result = -%inf

affich_result(sci2exp(%inf)=="%inf", 55.1)
affich_result(sci2exp(-%inf)=="-%inf", 55.2)
affich_result(sci2exp(%nan)=="%nan", 55.3)
affich_result(sci2exp(-%nan)=="%nan", 55.4)
