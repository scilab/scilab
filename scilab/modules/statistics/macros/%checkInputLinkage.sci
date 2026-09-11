// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function n = %checkInputLinkage(x, fname)
    // internal function 
    // used in linkage and agglomerative functions
    
    if isvector(x) then
        // x is a condensed distance vector
        m = length(x);

        // m = n*(n-1)/2
        n = (1 + sqrt(1 + 8*m)) / 2;

        if abs(n - round(n)) > sqrt(%eps) then
            error(msprintf(_("%s: Wrong size for input argument #%d: A condensed distance vector expected.\n"), fname, 1));
        end

        n = round(n);
    else
        // x is an observation matrix
        n = size(x, 1);

        if n < 2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: Must contain at least two observations (two rows).\n"), fname, 1));
        end

        if or(method == ["centroid", "median", "ward"]) && ...
            ((typeof(metric) == "string" && metric ~= "euclidean") || (typeof(metric) == "ce" && metric{1} ~= "euclidean")) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Euclidean metric with %s method expected.\n"), fname, 3, sci2exp(["centroid", "median", "ward"])));
        end
    end
endfunction
