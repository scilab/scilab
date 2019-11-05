// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = airy(k, z, scaled)

    fname = "airy"

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    rhs = argn(2)
    select rhs
    case 1
        z = k
        k = 0
        scaled = %f
    case 2
        scaled = %f
    case 3
        if ~isdef("scaled","l")
            scaled = %f
        else
            scaled = ~(~scaled)
        end
    else
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 1, 3))
    end

    // k
    if and(type(k) <> [1 8 10]) then
        msg = _("%s: Argument #%d: Integer number or string expected.\n")
        error(msprintf(msg, fname, 1))
    end
    if type(k)==10 then
        k = convstr(k(1))
        k = find(k==["ai" "dai" "bi" "dbi"])
        if k <> []
            k = k - 1
        end
    else
        k = real(k(1))
    end
    if and(k <> [0:3]) then
        msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
        error(msprintf(msg, fname, 1, """Ai"",""Bi"",""dAi"",""dBi"",0,1,2,3"))
    end
    // z
    if type(z) <> 1 then
        msg = _("%s: Argument #%d: Decimal or complex number expected.\n")
        error(msprintf(msg, fname, min(rhs,2)))
    end
    // Hypermatrices: reshape for bessel*()
    s = size(z)
    if length(s) > 2 then
        z = z(:)
    end

    // PROCESSING
    // ----------
    y = z
    p = real(z)>0
    n = ~p
    zeta(p) = 2/3*(z(p).^1.5)
    zeta(n) = 2/3*((-z(n)).^1.5)

    select k
    case 0 // Ai : OK
        y(p) = sqrt(z(p)/3)/%pi .* besselk(1/3, zeta(p))
        y(n) = sqrt(-z(n)/9) .* (besselj(1/3, zeta(n)) + besselj(-1/3,zeta(n)))
        y(z==0) = 1 / (3^(2/3) * gamma(2/3))

    case 1  // Ai' : OK
        y(p) = -z(p)/(%pi*sqrt(3)) .* besselk(2/3, zeta(p))
        y(n) =  z(n)/3 .* (besselj(-2/3, zeta(n)) - besselj(2/3,zeta(n)))
        y(z==0) = -1 / (3^(1/3) * gamma(1/3))

    case 2  // Bi : OK
        y(p) =   sqrt(z(p)/3) .* (besseli(1/3, zeta(p)) + besseli(-1/3, zeta(p)))
        y(n) = - sqrt(-z(n)/3) .* (besselj(1/3, zeta(n)) - besselj(-1/3, zeta(n)))
        y(z==0) = 1 / (3^(1/6) * gamma(2/3))

    case 3  // Bi' : OK
        y(p) =  z(p)/sqrt(3) .* (besseli(2/3, zeta(p)) + besseli(-2/3, zeta(p)))
        y(n) = -z(n)/sqrt(3) .* (besselj(2/3, zeta(n)) + besselj(-2/3,zeta(n)))
        y(z==0) = 3^(1/6) / gamma(1/3)
    end

    // Scaling
    // -------
    if scaled then
        if k < 2
            y =  y .* exp(2/3 * z.^1.5);
        else
            y = y .* exp(- 2/3 * abs(real(z.^1.5)));
        end
    end

    // Reshape the result
    // ------------------
    if length(s)>2 then
        y = matrix(y, s)
    end
endfunction
