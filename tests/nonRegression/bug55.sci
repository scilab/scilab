// bug55

mode (-1)
clear

affich_result(sci2exp(%inf)=="%inf", 55.1)
affich_result(sci2exp(-%inf)=="-%inf", 55.2)
affich_result(sci2exp(%nan)=="%nan", 55.3)
affich_result(sci2exp(-%nan)=="%nan", 55.4)

clear

// Error message :
// The sci2exp function gives wrong result with the -%inf value.
// sci2exp(-%inf) = -Inf instead of the right result = -%inf

