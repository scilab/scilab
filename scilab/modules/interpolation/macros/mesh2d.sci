//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2018 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [tri, hull]=mesh2d(varargin)
    if argn(2) == 0 then
        x = rand(10,1,"normal");
        y = rand(10,1,"normal");
        [tri, hull]=mesh2d(x,y);
        tri = [tri;tri(1,:)];
        plot(x,y,'o');
        xstring(x,y,string(1:size(x,"*")));
        xpolys(matrix(x(tri),size(tri)),matrix(y(tri),size(tri)));
        plot(x(hull),y(hull),'-r');
        return
    end
    if argn(2) < 2 || argn(2) > 3
        error(msprintf(_("%s: Wrong number of input argument(s): %d or %d expected.\n"), "mesh2d", 2, 3));
    end
    if argn(2) == 3 && argn(1) <> 1
        error(msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "mesh2d", 1));
    end
    if argn(2) == 2 && argn(1) > 2
        error(msprintf(_("%s: Wrong number of output argument(s): %d or %d expected.\n"), "mesh2d", 1, 2));
    end
    for i = 1:length(varargin)
        if type(varargin(i)) <> 1 || ~isreal(varargin(i))
            error(msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"), "mesh2d", i));
        end
    end
    if (size(varargin(1),"*") <> size(varargin(2),"*"))
        error(msprintf(_("%s: Arguments #%d and #%d must have the same sizes.\n"), "mesh2d", 1,2));
    end
    if (size(varargin(1),"*") < 3)
        error(msprintf(_("%s: Wrong size for input argument #%d : At least %d elements expected.\n"), "mesh2d)", 1, 3));
    end
    if argn(2) == 2
        [hull, area] = quickhull(varargin(1), varargin(2));
        if area < %eps
            error(msprintf(_("%s: all points are aligned.\n"),"mesh2d"))
        end
        tri = mesh2di(varargin(1), varargin(2), hull);
    else
        tri = mesh2di(varargin(1), varargin(2), varargin(3));
    end
end

function [ind, area]=quickhull(x, y)
    // https://fr.wikipedia.org/wiki/Quickhull
    // Quickhull is a method of computing the convex hull of a finite set of points in the plane.
    // It uses a divide and conquer approach similar to that of quicksort, from which its name derives.
    // Its best case complexity is considered to be O(n * log(n))
    // The following implementation supports all degenerates cases.

    // Determine vertices of minimum and maximum abcissa
    [minx,imin] = min(x(:));
    [maxx,imax] = max(x(:));
    if maxx-minx < %eps * (1 + max(maxx,minx))
      [miny,imin] = min(y(:));
      [maxy,imax] = max(y(:));
      if maxy-miny < %eps * (1 + max(maxy,miny))
        error(msprintf(_("%s: all points are coincident.\n"),"mesh2d"))
      end
    end  
    
    ind = 1:size(x,"*");
    ind([imin;imax]) = [];
    // Call recursive divide and conquer function
    // 1-for vertices under segment [imin imax]
    [ind1,remain] = conquer(x(:),y(:),ind,[imin;imax]);
    if ~isempty(remain) then
        // 2-for vertices (if any) above segment [imin imax]
        ind2 = conquer(x(:),y(:),remain,[imax;imin]);
        ind = [ind1(1:$-1);ind2];
    else // if all vertices were under segment [imin imax]
        ind=[ind1;imin];
    end
    area = sum(x(ind(1:$-1)).*y(ind(2:$))-x(ind(2:$)).*y(ind(1:$-1)))/2
end

function [out, remain]=conquer(x, y, ind, indH)
    out = indH;
    xh = x(indH);
    yh = y(indH);
    // compute signed distance w.r.t to segment [indH(1) indH(2)]
    u=[xh(2)-xh(1);yh(2)-yh(1)];
    d = (y(ind)-yh(1))*u(1)-(x(ind)-xh(1))*u(2);
    // 1-divide
    // ind(iplus) = vertices to the right of segment [indH(1) indH(2)]
    iplus = find(d<=0);
    remain = ind;
    remain(iplus) = [];
    if ~isempty(iplus) then
        // #indnew = the furthest vertex to the right of segment [indH(1),indH(2)
        [ma,ima]=min(d(iplus));
        if ma > -%eps // all the righ-of-segment vertices are *on* the segment
            iOnSeg = ind(iplus);
            // we sort vertices by increasing L_1 distance from indH(1)
            [sorted,iSorted] = gsort(abs(x(iOnSeg)-x(indH(1)))+...
                                     abs(y(iOnSeg)-y(indH(1))),'g','i');
            out = [indH(1);iOnSeg(iSorted)';indH(2)]
            return
        end
        indnew = ind(iplus(ima));
        iplus(ima) = [];
        // 2-conquer
        if ~isempty(iplus) then
            // process ind(iplus) vertices w.r.t. segment [indH(1) indnew]
            [ind1,remainplus]=conquer(x,y,ind(iplus),[indH(1);indnew]);
            if ~isempty(remainplus) then
                // process remaining vertices w.r.t. segment [indnew indH(2)]
                ind2=conquer(x,y,remainplus,[indnew;indH(2)]);
                out = [ind1(1:$-1);ind2];
            else
                out = [ind1;indH(2)];
            end
        else // #indnew vertex was alone to the right of [indH(1) indH(2)]
            out = [indH(1);indnew;indH(2)];
        end
    end
end

