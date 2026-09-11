// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function z = %linkage(x, n, method, metric, nClusters)
    // internal function 
    // used in linkage and agglomerative functions
    if ~isvector(x) then
        if typeof(metric) == "ce" then
            x = pdist(x, metric{:}(:));
        else
            x = pdist(x, metric);
        end
    end

    n_merges = n - nClusters;

    D = squareform(x, "tomatrix");

    // not use the diagonal
    D(1:n+1:n*n) = %inf;

    z = zeros(n_merges, 3);
    clusterId = 1:n;
    clusterSize = ones(n, 1);
    nextId = n + 1;

    for i = 1:n_merges
        [dmin, idx] = min(D(:));
        [r, c] = ind2sub(size(D), idx);

        idr = clusterId(r);
        idc = clusterId(c);

        z(i, :) = [min(idr,idc), max(idr,idc), dmin];

        nr = clusterSize(r);
        nc = clusterSize(c);
        newSize = nr + nc;

        index = 1:size(D, 1);
        index([r, c]) = [];

        // last merge: there is no longer any distance to compute
        if isempty(index) then
            break;
        end

        // distance from the new cluster to the others
        d = D(index, [r, c]);

        select method
        case "single"
            // d(U,k) = min(d(r,k), d(c,k))
            newd = min(d, "c");

        case "complete"
            // d(U,k) = max(d(r,k), d(c,k))
            newd = max(d, "c");

        case "average"
            // d(U,k) = nr*d(r,k) + nc*d(c,k)) / (nr + nc)
            w = clusterSize([r, c]);
            newd = (d * w) ./ newSize;

        case "weighted"
            // d(U,k) = (d(r,k) + d(c,k)) / 2
            newd = sum(d, "c")/2;

        case "centroid"
            // d(U,k)^2 = nr/(nr+nc) * d(r,k)^2 + nc/(nr+nc) * d(c,k)^2 - nr*nc/(nr+nc)^2 * d(r,c)^2
            dd = d.^2;
            alpha1 = nr / newSize;
            alpha2 = nc / newSize;
            beta1 = (nr * nc) / (newSize^2);
            newd = sqrt(alpha1 * dd(:, 1) + alpha2 * dd(:, 2) - beta1 * D(r, c)^2);

        case "median"
            // d(U,k)^2 = 1/2 * d(r,k)^2 + 1/2 * d(c,k)^2 - 1/4 * d(r,c)^2
            dd = d.^2;
            newd = sqrt(0.5 * dd(:,1) + 0.5 * dd(:, 2) - 0.25 * D(r, c)^2);

        case "ward"
            // d(U,k)^2 = ((nr + nk)*d(r,k)^2 +(nc + nk)*d(c,k)^2 - nk*d(r,c)^2) / (nr + nc + nk)
            dd = d.^2;
            nk = clusterSize(index);
            den = nr + nc + nk;
            alpha1 = (nr + nk);
            alpha2 = (nc + nk);
            newd = sqrt((alpha1 .* dd(:, 1) + alpha2 .* dd(:, 2) -  nk .* (D(r, c)^2)) ./den);     

        end       

        // update distances matrix
        D(r, index) = newd;
        D(index, r) = newd;

        // remove cluster c
        D(c, :) = [];
        D(:, c) = [];

        // update identifiers
        clusterId(r) = nextId;
        clusterId(c) = [];

        // update sizes
        clusterSize(r) = newSize;
        clusterSize(c) = [];

        nextId = nextId + 1;        
    end
endfunction
