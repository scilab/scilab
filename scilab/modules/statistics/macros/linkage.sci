// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function z = linkage(x, method, metric)
    arguments
        x {mustBeA(x, "double")}
        method {mustBeA(method, "string"), mustBeScalar, mustBeMember(method, ["single", "complete", "average", "weighted", "centroid", "median", "ward"])} = "single"
        metric {mustBeA(metric, ["string", "cell"])} = "euclidean"
    end

    n = %checkInputLinkage(x, "linkage");

    z = %linkage(x, n, method, metric, 1);

endfunction

