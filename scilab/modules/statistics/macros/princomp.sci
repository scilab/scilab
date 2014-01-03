// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function [facpr,comprinc,lambda,tsquare]=princomp(x,eco)
    //
    //This  function performs  several  computations known  as
    //"principal component  analysis".
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

    //References: Saporta, Gilbert, Probabilites,  Analyse des
    //Donnees et Statistique, Editions Technip, Paris, 1990.
    //
    //derived from pca,
    //  author: carlos klimann
    //

    [lhs, rhs] = argn(0);
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"princomp",1,2))
    end
    if rhs<2 then eco=%f,end
    if type(eco)<>4 then
        error(msprintf(_("%s: Wrong type for input argument #%d: Boolean expected."),"princomp",2))
    end

    if x==[] then
        facpr=[];
        comprinc=[];
        lambda=[];
        tsquare=[];
        return;
    end

    [rowx,colx]=size(x)

    //center data
    y=center(x,1)

    //compute eigenvectors of  y'*y using svd
    if eco then
        [U,lambda,facpr]=svd(y,"e");
    else
        [U,lambda,facpr]=svd(y);
    end
    lambda=diag(lambda).^2/(rowx-1);//normalize
    comprinc=y*facpr
    if rowx <= colx then
        if eco then
            lambda(rowx)=[]
            facpr(:,rowx)=[];
            comprinc(:,rowx) = [];
        else
            lambda(rowx:$)=[]
            comprinc(:,rowx:$) = [];
        end
    end
    q=find(lambda<=max(rowx,colx)*%eps*lambda(1),1)
    if q==[] then q=size(lambda,"*"),end
    tsquare=(rowx-1)*sum(U(:,1:q).^2,2)
endfunction
