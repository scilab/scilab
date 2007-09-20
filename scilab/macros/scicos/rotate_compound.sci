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
//**          gcf(curwin)
//**
//** 25/07/07, Alan
// Copyright INRIA
function rotate_compound(sel_x, sel_y, sel_w, sel_h, blk, theta)

  gh_curwin=gcf(curwin)
  o_size = size(gh_curwin.children.children);

  unglue(gh_curwin.children.children(blk))
  p_size = size(gh_curwin.children.children); //** size after the unglue
                                              //** objects unglued are
                                              //** in the top positions
  d_size =  p_size(1) - o_size(1);

  for i=1:d_size+1
     select gh_curwin.children.children(i).type
         case "Rectangle" then
            //disp('rectangle')

            //**rectangle is replaced by a polyline
            x=gh_curwin.children.children(i).data(1);
            y=gh_curwin.children.children(i).data(2);
            w=gh_curwin.children.children(i).data(3);
            h=gh_curwin.children.children(i).data(4);
            xxx = rotate([x, x  , x+w, x+w;...
                          y, y-h, y-h, y],theta*%pi/180,...
                         [sel_x+sel_w/2;sel_y-sel_h/2])
            xpoly(xxx(1,:),xxx(2,:),"lines",1);

            gh_e = gce()
            if i==1&blk==1 then //** select box
              gh_e.mark_background = -1;
              gh_e.mark_style = 11;
              gh_e.mark_mode = "off"; //** used
              gh_e.line_mode = "off";
            else
              gh_e.mark_background = gh_curwin.children.children(i+1).mark_background
              gh_e.mark_mode = gh_curwin.children.children(i+1).mark_mode
              gh_e.mark_style = gh_curwin.children.children(i+1).mark_style
              gh_e.mark_size = gh_curwin.children.children(i+1).mark_size
              gh_e.mark_background = gh_curwin.children.children(i+1).mark_background
              gh_e.mark_foreground = gh_curwin.children.children(i+1).mark_foreground

              gh_e.background = gh_curwin.children.children(i+1).background
              gh_e.foreground = gh_curwin.children.children(i+1).foreground
              gh_e.visible = gh_curwin.children.children(i+1).visible
              gh_e.line_mode = gh_curwin.children.children(i+1).line_mode
              gh_e.fill_mode = gh_curwin.children.children(i+1).fill_mode
              gh_e.line_style = gh_curwin.children.children(i+1).line_style
              gh_e.thickness = gh_curwin.children.children(i+1).thickness
            end

            swap_handles(gh_curwin.children.children(i+1),...
                          gh_curwin.children.children(1));
            delete(gh_curwin.children.children(1));


         case "Text" then
            //disp('text')

            //** get bounding box of text with no rotation
            rect = stringbox(gh_curwin.children.children(i).text,gh_curwin.children.children(i).data(1),...
                             gh_curwin.children.children(i).data(2))
            x=rect(1,2);
            y=rect(2,2);
            w=rect(1,3)-rect(1,2);
            h=rect(2,2)-rect(2,4);

            xxx = rotate([x, x  , x+w, x+w;...
                          y, y-h, y-h, y],theta*%pi/180,...
                          [sel_x+sel_w/2;sel_y-sel_h/2])

            //** update lower left point of text
            gh_curwin.children.children(i).data = [xxx(1,2) xxx(2,2)]

            //**adjust theta according to gh_txt.font_angle
            if theta > 0 then
              gh_curwin.children.children(i).font_angle = 360 - theta
            else
              gh_curwin.children.children(i).font_angle = -theta
            end

         case "Polyline" then
            //disp('polyline')

            xxx=rotate([gh_curwin.children.children(i).data(:,1)';gh_curwin.children.children(i).data(:,2)'],...
                        theta*%pi/180,...
                       [sel_x+sel_w/2;sel_y-sel_h/2])
            gh_curwin.children.children(i).data = [xxx(1,:)',xxx(2,:)']

         case "Compound" then
            //disp('compound')

            rotate_compound(sel_x, sel_y, sel_w, sel_h,i,theta)

            //** retrieve original place of compound
            //** in children of axe
            for j=1:i-1
              swap_handles(gh_curwin.children.children(j),... 
                           gh_curwin.children.children(j+1));
            end

         case "Segs" then
            //disp('Segs')

            xxx=rotate([gh_curwin.children.children(i).data(:,1)';gh_curwin.children.children(i).data(:,2)'],...
                        theta*%pi/180,...
                       [sel_x+sel_w/2;sel_y-sel_h/2])
            gh_curwin.children.children(i).data = [xxx(1,:)',xxx(2,:)']


         case "Arc" then
            //disp('Arc')

            x=gh_curwin.children.children(i).data(1);
            y=gh_curwin.children.children(i).data(2);
            w=gh_curwin.children.children(i).data(3);
            h=gh_curwin.children.children(i).data(4);

            xxx = rotate([x+w/2; y-h/2],theta*%pi/180,...
                         [sel_x+sel_w/2;sel_y-sel_h/2])

            gh_curwin.children.children(i).data(1) = xxx(1,1)-w/2
            gh_curwin.children.children(i).data(2) = xxx(2,1)+h/2
     end
  end

  glue(gh_curwin.children.children(d_size+1:-1:1));

endfunction