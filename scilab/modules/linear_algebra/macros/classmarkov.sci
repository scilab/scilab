
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [perm, rec, tr, indsRec, indsT] = classmarkov(M)
    //returns a permutation vector perm such that
    //M(perm,perm) = [M11 0 0 0 0   0]
    //               [0 M22 0 0     0]
    //               [0 0 M33       0]
    //               [      ...      ]
    //               [0 0       Mrr 0]
    //               [* *        *  Q]
    //Each Mii is a Markov matrix of dimension rec(i)  i=1,..,r
    //Q is sub-Markov matrix of dimension tr
    //States 1 to sum(rec) are recurrent and states from r+1 to n
    //are transient.
    //perm=[indsRec,indsT] where indsRec is a vector of size sum(rec)
    //and indsT is a vector of size tr.

    // Extract (tail, head) = positions of nonzero entries in M
    // find works on both dense and sparse matrices
    n = size(M, 1);
    [tail_, head_] = find(M <> 0);
    ma = size(tail_, '*');

    if ma == 0 then
        // No edges: each node is its own component
        nc = n;
        ncomp = 1:n;
    else
        // Build compressed adjacency lists (CSR) for directed graph
        [lp, lls] = compute_adjacency(tail_, head_, ma, n)

        // Find strongly connected components (Tarjan's algorithm)
        [nc, ncomp] = find_strongly_connected_components(lp, lls, ma, n);
    end

    indsRec=[]; indsT=[]; rec=[]; tr=0;
    for i=1:nc
        inds = find(ncomp==i);
        nb = size(inds, "*");
        M1 = M(inds,:); M1(:,inds) = [];
        if sum(M1)==0 then
            indsRec = [indsRec, inds];
            rec = [rec, nb];
        else
            indsT = [indsT, inds];
            tr = tr + nb;
        end
    end
    perm = [indsRec, indsT];
endfunction


function [lp, lls] = compute_adjacency(tail_, head_, ma, n)
    // Computes the adjacency vectors lp and lls
    // from vectors tail and head for a directed graph
    //
    // Input:  tail(1:ma) = source nodes of edges
    //         head(1:ma) = destination nodes of edges
    //         ma = number of edges
    //         n  = number of nodes
    // Output: lp(1:n+1) = row pointer vector
    //         lls(1:ma)  = successor (destination) nodes, sorted by source

    lp = zeros(1, n+1);
    lls = zeros(1, ma);

    // first computation of lp
    // lp(i+1) = number of tail nodes
    [u, _, _, nb] = unique(tail_ + 1);
    lp(u) = nb

    // second computation of lp
    // lp(i) = pointer to the first arc
    // with tail i in sorted tail
    lp(1) = 1;
    lp = [cumsum(lp(1:n)) lp($)];

    // computation of ls
    for iarc = 1:ma
        inode = tail_(iarc);
        lls(lp(inode)) = head_(iarc);
        lp(inode) = lp(inode) + 1;
    end

    // last computation of lp
    lp = [1 lp(1:n)];

endfunction


function [nc, nfcomp] = find_strongly_connected_components(lp, lls, m, n)
    // Finds all strongly connected components of a directed graph.
    // Uses Tarjan's algorithm via iterative DFS.
    //
    // Input:  lp(1:n+1) = CSR row pointers
    //         ls(1:m)   = CSR successor nodes
    //         m = number of edges
    //         n = number of nodes
    // Output: nc = number of strongly connected components
    //         nfcomp(1:n) = component ID for each node

    pw     = zeros(1, n);
    nn     = zeros(1, n);
    num    = zeros(1, n);
    nfcomp = zeros(1, n);
    inf_v  = zeros(1, n);
    pile   = zeros(1, n);

    nn = lp(2:$)-lp(1:$-1);

    nc = 1;
    i0 = 1;

    while %t
        // Run Tarjan DFS from node i0
        [nc, nfcomp, pw, nn, num, inf_v, pile] = strongconnect(i0, inf_v, lp, lls, m, n, nc, nfcomp, nn, num, pw, pile);

        // Find next unvisited node
        found = %f;
        i0 = find(nfcomp == 0, 1);
        if i0 <> [] then
            found = %t;
        end
        if ~found then break; end
    end

    nc = nc - 1;
endfunction


function [nc, nfcomp, pw, nn, num, inf_v, pile] = strongconnect(i0, inf_v, lp1, ls1, m, n, nc, nfcomp, nn, num, pw, pile)
    // Core of Tarjan's SCC algorithm — iterative DFS.

    pw(i0) = i0;
    k = 1;
    num(i0) = k;
    inf_v(i0) = k;
    i = i0;
    np = 1;
    pile(np) = i0;

    while %t
        if nn(i) == 0 then
            if inf_v(i) == num(i) then
                while pile(np) <> i
                    j = pile(np);
                    nfcomp(j) = nc;
                    np = np - 1;
                end
                nfcomp(i) = nc;
                np = np - 1;
                nc = nc + 1;
                if i == i0 then
                    pw(i0) = -pw(i0);
                    return;
                end
            end
            j = pw(i);
            pw(i) = -pw(i);
            inf_v(j) = min(inf_v(j), inf_v(i));
            i = j;
        else
            ll = lp1(i) + nn(i) - 1;
            j = ls1(ll);
            nn(i) = nn(i) - 1;

            if pw(j) < 0 then
                if nfcomp(j) == 0 then
                    inf_v(i) = min(inf_v(i), inf_v(j));
                end
            elseif pw(j) > 0 then
                inf_v(i) = min(num(j), inf_v(i));
            else
                pw(j) = i;
                k = k + 1;
                num(j) = k;
                np = np + 1;
                pile(np) = j;
                inf_v(j) = num(j);
                i = j;
            end
        end
    end
endfunction
