// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [d,ptp,ind,c]=orthProj(data,pt)
    // computes minimum distance from a point to a polyline
    //d    minimum distance of the point to the nearest polyline data point
    //ptp  projected point coordiantes
    //ind  projection lies on segment [ind ind+1]
    //c    orthogonal projection coefficient
    if argn(2)<>2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"orthProj",2))
    end


    d = [];ptp = [];ind = [],c = []
    [n,m] = size(data)
    pt = matrix(pt,1,-1) //make pt a row vector
    if n<2 then return,end
    //the orthogonal projection coefficient of the vector y on the vector x;
    //is given by  <x,y>/||x||^2
    //shift origin to (0,0) for each segment defined by data
    X = (data(2:$,:)-data(1:$-1,:))
    //apply similar origin transformation to the given point
    Y = ones(n-1,1)*pt-data(1:$-1,:) ;
    //compute the orthogonal projection coefficients relative to each segments
    //first remove zero length segements;
    L = sum(X.*X,2); //segment lengths
    nz = find(L>0)
    X = X(nz,:); Y = Y(nz,:);
    P = sum(X.*Y,2)./L(nz);
    //the projected point lies in the segment nz(i) if 0 <= P(i)<1
    i_in = find(P >= 0 & P<1); //find segments the projected point falls in

    if i_in<>[] then
        //find the segment that realizes the min distance
        [d,k] = min(sum((X(i_in,:).*(P(i_in)*ones(1,m))-Y(i_in,:)).^2,2))
        d = sqrt(d) //the mini distance between the given point and the curve
        i_in = i_in(k) //index of the first bound of the segment in data
        c = P(i_in)  // the orthogonal projection coefficient
        ind = nz(i_in) //make i_in relative to the initial data
        ptp = data(ind,:)+(data(ind+1,:)-data(ind,:))*c //the projected point
    end
endfunction
