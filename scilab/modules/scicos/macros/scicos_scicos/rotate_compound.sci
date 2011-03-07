//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

//** rotate compound : do a rotation of graphics elements
//** in a compound
//**
//** input : -sel_x, sel_y, sel_w, sel_h : give the coordinate
//**          of the box surrounding the compound
//**          (lower point,width, height).
//**          The rotation will be done from the center
//**          [sel_x+sel_w/2;sel_y-sel_h/2]
//**
//**         - blk the index of the compound (the i child of axe)
//**
//**         - theta : angle of rotation given in degree
//**                   in trigonometric wise
//**
//** output : nothing. This function directly work in the figure handle
//**          gh_ = scf(curwin)
//**
//** 25/07/07, Alan
function rotate_compound(sel_x, sel_y, sel_w, sel_h, blk, theta)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


  //** gh_curwin = gh_curwin;
  gh_curwin = scf(gh_curwin);
  gh_axes = gca();

  o_size = size(gh_axes.children);

  unglue(gh_axes.children(blk))
  p_size = size(gh_axes.children); //** size after the unglue
                                   //** objects unglued are
                                   //** in the top positions
  d_size =  p_size(1) - o_size(1);

  for i=1:d_size+1
     select gh_axes.children(i).type
         case "Rectangle" then
            //disp('rectangle')

            //**rectangle is replaced by a polyline
            x = gh_axes.children(i).data(1);
            y = gh_axes.children(i).data(2);
            w = gh_axes.children(i).data(3);
            h = gh_axes.children(i).data(4);
            xxx = rotate([x, x  , x+w, x+w;...
                          y, y-h, y-h, y ],theta*%pi/180,...
                         [sel_x+sel_w/2;sel_y-sel_h/2])
            xpoly(xxx(1,:),xxx(2,:),"lines",1);

            gh_e = gce()
            if (i==1) & (blk==1) then //** select box
              gh_e.mark_background = -1;
              gh_e.mark_style = 11;
              gh_e.mark_mode = "off"; //** used
              gh_e.line_mode = "off";

            else //** it is not the selection box 
            
              mark_mode_flag = gh_axes.children(i+1).mark_mode;
              //** Touch the mark proprieties ONLY if the mark_mode is ON ! 
              if mark_mode_flag=="on" then 
                gh_e.mark_mode  = gh_axes.children(i+1).mark_mode              
                gh_e.mark_style = gh_axes.children(i+1).mark_style
                gh_e.mark_size  = gh_axes.children(i+1).mark_size
                gh_e.mark_background = gh_axes.children(i+1).mark_background
                gh_e.mark_foreground = gh_axes.children(i+1).mark_foreground
              end 

              gh_e.background = gh_axes.children(i+1).background
              gh_e.foreground = gh_axes.children(i+1).foreground
              gh_e.visible    = gh_axes.children(i+1).visible
              gh_e.line_mode  = gh_axes.children(i+1).line_mode
              gh_e.fill_mode  = gh_axes.children(i+1).fill_mode
              gh_e.line_style = gh_axes.children(i+1).line_style
              gh_e.thickness  = gh_axes.children(i+1).thickness
            end

            swap_handles(gh_axes.children(i+1), gh_axes.children(1));
            delete(gh_axes.children(1)); //** delete the last object (top of stack) 

         case "Text" then
            //disp('text')

            //** get bounding box of text with no rotation
            //** rect = stringbox( string, x, y) 
            rect = stringbox(gh_axes.children(i).text, ...
                             gh_axes.children(i).data(1),... //** x
                             gh_axes.children(i).data(2) );  //** y
            x = rect(1,2);
            y = rect(2,2);
            w = rect(1,3)-rect(1,2);
            h = rect(2,2)-rect(2,4);

            xxx = rotate([x, x  , x+w, x+w;...
                          y, y-h, y-h, y],theta*%pi/180,...
                          [sel_x+sel_w/2;sel_y-sel_h/2])

            //** update lower left point of text
            gh_axes.children(i).data = [xxx(1,2) xxx(2,2)]

            //**adjust theta according to gh_txt.font_angle
            if theta > 0 then
              gh_axes.children(i).font_angle = 360 - theta ;
            else
              gh_axes.children(i).font_angle = -theta ;
            end

         case "Polyline" then
            //disp('polyline')

            xxx = rotate([gh_axes.children(i).data(:,1)';gh_axes.children(i).data(:,2)'],...
                         theta*%pi/180,...
                         [sel_x+sel_w/2;sel_y-sel_h/2]);
            
            gh_axes.children(i).data = [xxx(1,:)',xxx(2,:)']

         case "Compound" then
            //disp('compound')

            rotate_compound(sel_x, sel_y, sel_w, sel_h,i,theta)

            //** retrieve original place of compound
            //** in children of axe
            for j=1:i-1
              swap_handles(gh_axes.children(j), gh_axes.children(j+1));
            end

         case "Segs" then
            //disp('Segs')

            xxx = rotate([gh_axes.children(i).data(:,1)'; gh_axes.children(i).data(:,2)'],...
                         theta*%pi/180,...
                         [sel_x+sel_w/2;sel_y-sel_h/2])
            
            gh_axes.children(i).data = [xxx(1,:)',xxx(2,:)']


         case "Arc" then
            //disp('Arc')

            x = gh_axes.children(i).data(1);
            y = gh_axes.children(i).data(2);
            w = gh_axes.children(i).data(3);
            h = gh_axes.children(i).data(4);

            xxx = rotate([x+w/2; y-h/2],theta*%pi/180,...
                         [sel_x+sel_w/2; sel_y-sel_h/2])

            gh_axes.children(i).data(1) = xxx(1,1)-w/2 ;
            gh_axes.children(i).data(2) = xxx(2,1)+h/2 ;
     end
  end

  glue(gh_axes.children(d_size+1:-1:1)) ;

endfunction
