// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function labels = agglomerative(x, n_clusters, method, metric)
    arguments
        x {mustBeA(x, "double")}
        n_clusters {mustBeA(n_clusters, "double"), mustBeScalar, mustBeInteger}
        method {mustBeA(method, "string"), mustBeScalar, mustBeMember(method, ["single", "complete", "average", "weighted", "centroid", "median", "ward"])} = "single"
        metric {mustBeA(metric, ["string", "cell"])} = "euclidean"
    end  

    if %is_linkage(x) then
        Z = x;
        n = size(x, 1) + 1;

        if n < 2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: Must contain at least two observations.\n"), "agglomerative", 1));
        end

        if n_clusters < 1 || n_clusters > n then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be between 1 and %d.\n"), "agglomerative", 2, n));
        end

    else
        n = %checkInputLinkage(x, "agglomerative");

        if n < 2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: Must contain at least two observations.\n"), "agglomerative", 1));
        end

        if n_clusters < 1 || n_clusters > n then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be between 1 and %d.\n"), "agglomerative", 2, n));
        end

        Z = %linkage(x, n, method, metric, n_clusters);
    end

    n_merges = n - n_clusters;

    if n_merges == 0 then 
        labels = (0:n-1)'; 
        return; 
    end

    clusters = list();

    for i = 1:n
        clusters(i) = i;
    end
    
    active = ones(1, n + n_merges);


    for i = 1:n_merges

        id1 = Z(i, 1);
        id2 = Z(i, 2);

        newId = n + i;

        clusters(newId) = [clusters(id1), clusters(id2)];

        active(id1) = 0;
        active(id2) = 0;

        active(newId) = 1;

    end

    activeIds = find(active == 1);
    
    labels = zeros(n, 1);

    i = length(activeIds) - 1;
    for id = activeIds
        labels(clusters(id)) = i;
        i = i - 1;
    end

endfunction

function tf = %is_linkage(Z)
    tf = %f;

    if size(Z, 2) <> 3 || size(Z, 1) < 1 then
        return;
    end

    n = size(Z, 1) + 1;
    ids = Z(:, 1:2);
    if or(abs(ids - round(ids)) > sqrt(%eps)) || min(ids) < 1 then
        return;
    end

    u = unique(ids);
    if length(u) <> size(ids, "*") then
        return
    end

    if min(Z(:, 3)) < 0 then
        return;
    end

    tf = %t;

endfunction
