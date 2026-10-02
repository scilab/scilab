// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = silhouette(X, labels, metric, param)
    arguments
        X {mustBeA(X, "double")}
        labels {mustBeA(labels, "double"), mustBeVector}
        metric {mustBeA(metric, "string")} = "euclidean"
        param = []
    end

    n_samples = size(X, 1);

    if n_samples < 2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: At least 2 samples expected.\n"), "silhouette", 1));
    end

    if length(labels) <> n_samples then
        error(msprintf(_("%s: Wrong size for input argument #%d: a vector of length %d expected.\n"), "silhouette", 2, n_samples));
    end

    unique_labels = unique(labels);
    n_clusters = length(unique_labels);

    if n_clusters < 2 then
        error(msprintf(_("%s: Wrong value for input argument #%d: At least 2 clusters expected.\n"), "silhouette", 2));
    end

    if n_clusters >= n_samples then
        error(msprintf(_("%s: Wrong value for input argument #%d: Number of clusters must be < %d.\n"), "silhouette", 2, n_samples));
    end

    if param == [] then
        v = list(metric);
    else
        v = list(metric, param);
    end

    d = pdist(X, v(:));
    D = squareform(d);

    out = zeros(n_samples, 1);

    same_cluster = list();
    idx = 1:n_samples;
    for i = 1:n_clusters
        v = labels == unique_labels(i);
        same_cluster($+1) = idx(v);
    end

    for i = 1:n_samples
        current_cluster = labels(i);
        same_cluster_idx = find(labels == current_cluster);
        same_cluster_idx(same_cluster_idx == i) = [];

        // a(i): mean intra-cluster distance
        if isempty(same_cluster_idx) then
            a = 0;
        else
            a = mean(D(i, same_cluster_idx));
        end

        // b(i): minimum mean distance to another cluster
        b = %inf;
        for k = 1:n_clusters
            other_cluster = unique_labels(k);
            if other_cluster == current_cluster then
                continue;
            end

            other_cluster_idx = same_cluster(k)
            m = mean(D(i, other_cluster_idx));
            if m < b then
                b = m;
            end
        end

        if a <> 0 | b <> 0 then
            out(i) = (b - a) / max(a, b);
        end
    end
endfunction
