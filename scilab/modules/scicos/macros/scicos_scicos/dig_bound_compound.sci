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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
// Copyright INRIA
function [rect]=dig_bound_compound(blk)

  gh_curwin=gh_curwin;
  o_size = size(gh_curwin.children.children);

  unglue(gh_curwin.children.children(blk))
  p_size = size(gh_curwin.children.children); //** size after the unglue
                                              //** objects unglued are
                                              //** in the top positions
  d_size =  p_size(1) - o_size(1);

  xmin = 100000;
  xmax = -xmin;
  ymin = xmin;
  ymax = -xmin;

  for i=1:d_size+1
     select gh_curwin.children.children(i).type
         case "Rectangle" then
            //disp('rectangle')

            xmin=min(xmin,gh_curwin.children.children(i).data(1));
            ymin=min(ymin,gh_curwin.children.children(i).data(2));
            xmax=max(xmax,gh_curwin.children.children(i).data(3)+gh_curwin.children.children(i).data(1));
            ymax=max(ymax,gh_curwin.children.children(i).data(4)+gh_curwin.children.children(i).data(2));

         case "Text" then
            //disp('text')

            //** get bounding box of text with no rotation
            rectstr = stringbox(gh_curwin.children.children(i).text,gh_curwin.children.children(i).data(1),...
                             gh_curwin.children.children(i).data(2))

            xmin=min(xmin,rectstr(1,1));
            ymin=min(ymin,rectstr(2,1));
            xmax=max(xmax,rectstr(1,3));
            ymax=max(ymax,rectstr(2,3));

         case "Polyline" then
            //disp('polyline')

            xmin=min(xmin,min(gh_curwin.children.children(i).data(:,1)));
            ymin=min(ymin,min(gh_curwin.children.children(i).data(:,2)));
            xmax=max(xmax,max(gh_curwin.children.children(i).data(:,1)));
            ymax=max(ymax,max(gh_curwin.children.children(i).data(:,2)));

         case "Compound" then
            //disp('compound')

            [rectcmpd]=dig_bound_compound(i)
            xmin=min(xmin,rectcmpd(1,1));
            ymin=min(ymin,rectcmpd(1,2));
            xmax=max(xmax,rectcmpd(1,3));
            ymax=max(ymax,rectcmpd(1,4));

         case "Segs" then
            //disp('Segs')

            xmin=min(xmin,min(gh_curwin.children.children(i).data(:,1)));
            ymin=min(ymin,min(gh_curwin.children.children(i).data(:,2)));
            xmax=max(xmax,max(gh_curwin.children.children(i).data(:,1)));
            ymax=max(ymax,max(gh_curwin.children.children(i).data(:,2)));

         case "Arc" then
            //disp('Arc')

            xmin=min(xmin,gh_curwin.children.children(i).data(1));
            ymin=min(ymin,gh_curwin.children.children(i).data(2));
            xmax=max(xmax,gh_curwin.children.children(i).data(3)+gh_curwin.children.children(i).data(1));
            ymax=max(ymax,gh_curwin.children.children(i).data(4)+gh_curwin.children.children(i).data(2));
     end
  end

  rect=[xmin ymin xmax ymax]

  glue(gh_curwin.children.children(d_size+1:-1:1));

endfunction
