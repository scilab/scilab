//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
//** rotate compound : get dig_bound of a compound
//**
//** input : - blk the index of the compound (the i child of axe)
//**         -
//** output : rect
//**
//** 25/11/08, Alan
//** 22/06/09, Serge Steer INRIA
//  - input argument replaced by the compound handle
//  - removed the useless unglue + glue instruction using the compuound
//    children list
// Copyright INRIA
function [rect]=dig_bound_compound(gh_blk)
    //gh_blk handle on a compound object

    xmin = 100000;
    xmax = -xmin;
    ymin = xmin;
    ymax = -xmin;
    C=gh_blk.children
    for i=1:size(C,"*")
        select C(i).type
        case "Rectangle" then
            //disp('rectangle')

            xmin=min(xmin,C(i).data(1));
            ymin=min(ymin,C(i).data(2));
            xmax=max(xmax,C(i).data(3)+C(i).data(1));
            ymax=max(ymax,C(i).data(4)+C(i).data(2));

        case "Text" then
            //disp('text')

            //** get bounding box of text with no rotation
            rectstr = stringbox(C(i).text,C(i).data(1),...
            C(i).data(2))

            xmin=min(xmin,rectstr(1,1));
            ymin=min(ymin,rectstr(2,1));
            xmax=max(xmax,rectstr(1,3));
            ymax=max(ymax,rectstr(2,3));

        case "Polyline" then
            //disp('polyline')

            xmin=min(xmin,min(C(i).data(:,1)));
            ymin=min(ymin,min(C(i).data(:,2)));
            xmax=max(xmax,max(C(i).data(:,1)));
            ymax=max(ymax,max(C(i).data(:,2)));

        case "Compound" then
            //disp('compound')

            [rectcmpd]=dig_bound_compound(C(i))
            xmin=min(xmin,rectcmpd(1,1));
            ymin=min(ymin,rectcmpd(1,2));
            xmax=max(xmax,rectcmpd(1,3));
            ymax=max(ymax,rectcmpd(1,4));

        case "Segs" then
            //disp('Segs')

            xmin=min(xmin,min(C(i).data(:,1)));
            ymin=min(ymin,min(C(i).data(:,2)));
            xmax=max(xmax,max(C(i).data(:,1)));
            ymax=max(ymax,max(C(i).data(:,2)));

        case "Arc" then
            //disp('Arc')

            xmin=min(xmin,C(i).data(1));
            ymin=min(ymin,C(i).data(2));
            xmax=max(xmax,C(i).data(3)+C(i).data(1));
            ymax=max(ymax,C(i).data(4)+C(i).data(2));
        end
    end

    rect=[xmin ymin xmax ymax]

endfunction
