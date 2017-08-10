// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function h=ged_loop(a,pt)

    h=[]
    minDist    = 0.01 ;
    minPixDist = 3    ;

    for ka=1:size(a,"*")
        ck=a(ka) ;
        select ck.type

        case "Polyline"
            xy=ck.data;
            d=Dist2polyline((xy(:,1)-Xmin)/Dx,(xy(:,2)-Ymin)/Dy,pts)
            if d < minDist then h=ck,return,end

        case "Rectangle"
            xy=ck.data;
            x0=xy(1);y0=xy(2);W=xy(3);H=xy(4);
            d=Dist2polyline((x0+[0,W,W,0]-Xmin)/Dx,(y0+[0,0,-H,-H]-Ymin)/Dy,pts)
            if d < minDist then h=ck,return,end

        case "Arc"
            xy=ck.data;
            [xp,yp]=xchange(pt(1),pt(2),"i2f")
            //[dist, toto] = dist2Arc( [xp,yp] ./ [Dx,Dy], xy(1:2)./[Dx,Dy], xy(3)/Dx, xy(4)/Dy, xy(5) / 64., xy(6) / 64. ) ;
            dist = pixDist2Arc( [xp,yp], xy(1:2), xy(3), xy(4), xy(5) / 64., xy(6) / 64. ) ;
            if dist <= minPixDist then
                h=ck;
                return;
            end

        case "Segs"
            xy=ck.data;
            xv=(matrix(xy(:,1),2,-1)-Xmin)/Dx
            yv=(matrix(xy(:,2),2,-1)-Ymin)/Dy
            for ks=1:size(xv,2)
                d=Dist2polyline(xv(:,ks),yv(:,ks),pts)
                if d < minDist then h=ck,return,end
            end
        case "Compound"
            h=ged_loop(ck.children,pt)
            if h<>[] then return,end

        case "Axes"
            xy=ck.data_bounds;
            [xp,yp]=xchange(pt(1),pt(2),"i2f")
            Xmin=xy(1,1);Ymin=xy(1,2),Dx=xy(2,1)-xy(1,1);Dy=xy(2,2)-xy(1,2);
            pts=[(xp-Xmin)/Dx (yp-Ymin)/Dy]
            d=Dist2polyline([0,1,1,0],[0,0,1,1],pts)
            if d < minDist then h=ck,return,end
            h=ged_loop([a.children(:);ck.x_label;ck.y_label;ck.z_label;ck.title],pt)
            if h<>[] then return,end

        case "Text"
            if is_in_text(ck,[xp;yp]) then
                h=ck,
                return,
            end

        case "Label"
            if is_in_text(ck,[xp;yp]) then
                h=ck
                return,
            end

        end
    end
endfunction
