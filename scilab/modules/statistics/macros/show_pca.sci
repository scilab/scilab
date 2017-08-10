// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
function show_pca(lambda,facpr,N)
    //used to visualize Principal component analysis results.
    // It includes  drawing of
    //"correlations circle",  i.e. in the  horizontal axis the
    //correlation   values  r(c1;xj)   and  in   the  vertical
    //r(c2;xj). It is an extension of the pca function.

    //References: Saporta, Gilbert, Probabilites,  Analyse des
    //Donnees et Statistique, Editions Technip, Paris, 1990.
    //
    //Extracted out of pca, author: carlos klimann

    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"show_pca",2,3))
    end
    if rhs<3 then
        N=[1 2]
    end
    if size(N,"*")<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Size %s expected.\n"),"show_pca",2,"2x1")),
    end,

    colx=size(facpr,2)
    if max(N)>colx then
        error(msprintf(gettext("%s: Graph demand out of bounds.\n"),"pca"))
    end

    rango=find(lambda(:,1)<=size(facpr,2)*%eps*lambda(1,1),1)
    if  rango==[] then
        rango=size(lambda,1)
    end


    rc = (ones(colx,1)* sqrt((lambda(N,1))')) .* facpr(:,N) ;
    ra = [1:rango]';
    if ( rango <= 1 ) then
        return
    end

    // Plot the correlation circle
    subplot(121)
    plot(rc(ra,1),rc(ra,2),"b*");
    for k=1:rango
        xstring(rc(k,1),rc(k,2),"X"+string(k));
    end
    xtitle(gettext("Correlations Circle"),"Axis 1","Axis 2");
    h = gcf()
    h.children.data_bounds=[-1,-1;1,1]
    // Plot the circle
    t = linspace(0,2*%pi,1000);
    plot(cos(t),sin(t),"r-")
    // Set isoview and grid
    isoview()
    gca().grid = [31 31]

    // Plot the ratio of each eigenvalue over the sum.
    subplot(122)
    bar(lambda(:,2))
    plot(ra,cumsum(lambda(:,2)),"r*-")
    xtitle(gettext("Eigenvalues"),"Indice","Ratio")
endfunction
