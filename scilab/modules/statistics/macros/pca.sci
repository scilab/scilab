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

function [lambda,facpr,comprinc]=pca(x)
    //
    //This  function performs  several  computations known  as
    //"principal component  analysis".
    //
    //The  idea  behind this  method  is  to  represent in  an
    //approximative  manner a  cluster of  n individuals  in a
    //smaller  dimensional subspace.  In order  to do  that it
    //projects the cluster onto a subspace.  The choice of the
    //k-dimensional projection subspace is  made in such a way
    //that  the distances  in  the projection  have a  minimal
    //deformation: we are looking for a k-dimensional subspace
    //such that the squares of the distances in the projection
    //is  as  big  as  possible  (in  fact  in  a  projection,
    //distances can only stretch).  In other words, inertia of
    //the projection  onto the k dimensional  subspace must be
    //maximal.
    //
    //x is a nxp (n  individuals, p variables) real matrix.
    //
    //lambda is a px2 numerical matrix. In the first column we
    //find the  eigenvalues of V,  where V is  the correlation
    //pxp matrix  and in the  second column are the  ratios of
    //the   corresponding   eigenvalue   over   the   sum   of
    //eigenvalues.
    //
    //facpr  are  the principal  factors:  eigenvectors of  V.
    //Each  column is an  eigenvector element  of the  dual of
    //R^p. Is an orthogonal matrix.
    //
    //comprinc  are  the  principal components.   Each  column
    //(c_i=Xu_i)  of  this  nxn  matrix  is  the  M-orthogonal
    //projection of individuals onto principal axis.  Each one
    //of this columns is a linear combination of the variables
    //x1,  ...,xp   with  maximum  variance   under  condition
    //u'_iM^(-1)u_i=1.
    //
    //Verification: comprinc*facpr=x
    //
    //References: Saporta, Gilbert, Probabilites,  Analyse des
    //Donnees et Statistique, Editions Technip, Paris, 1990.
    //
    //author: carlos klimann
    //
    //date: 2002-02-05
    //commentary fixed 2003-19-24 ??
    // update disable graphics output Allan CORNET 2008
    // request user
    // splitted in printcomp (matlab compatibility) and show_pca Serge Steer
    // 2008

    if argn(2) == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"pca",1))
    end
    if x==[] then
        lambda=%nan;
        facpr=%nan;
        comprinc=%nan;
        return;
    end
    [facpr,comprinc,lambda]=princomp(wcenter(x,1))
    lambda(:,2)=lambda(:,1)/sum(lambda(:,1))
endfunction
