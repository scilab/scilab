
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function e=ge_draw_edge(n)

//Construct the graphic object associated with a new edge, the first
  
  Id=ge_get_edge_id(n)

  // update default settings
  a = gca();

  // default settings for polylines and arcs
  [edge_width,edge_color,style]=ge_get_edge_style(n)
  if (edge_width <> a.thickness) then
     a.thickness = edge_width;
  end
  if(edge_color <> a.foreground) then
    a.foreground = edge_color;
  end

  // default settings for strings
  font=ge_get_edge_font(n)
  if (font(3) <> a.font_foreground) then
     a.font_foreground = font(3);
  end
  if(font(2) <> a.font_style) then
    a.font_style = font(2);
  end
  if(font(1) <> a.font_size) then
    a.font_size = font(1);
  end

  if GraphList.edges.tail(n)==GraphList.edges.head(n) then //loop arc
    
    // Draw the arc path

    [arc,xya,xyl,ang]=ge_profile2real(n)
    if size(arc,'*')==6 then
      xarc(arc(1),arc(2),arc(3),arc(4),arc(5),arc(6));e1=gce();
    else
      xpoly(arc(:,1),arc(:,2));e1=gce();
    end
    ang=0
  else //arc between two distinct nodes
    [xyc,xya,xyl,ang]=ge_profile2real(n)
    // Draw the arc path
    xpoly(xyc(:,1),xyc(:,2));e1=gce();
    
  end

  // Draw the arc arrow
  xpoly(xya(:,1),xya(:,2));e2=gce();
  if ~GraphList.directed then e2.visible='off',end
  
  // Draw the arc Id
  if Id==[] then
    Id='',
    r = 0;
  else
    r = stringbox(Id, 0, 0, 0, font(2), font(1));
    r = abs(r(1,3) - r(1,1));
  end
  if ang>90 & ang<270 then //co negatif
    a=ang*%pi/180
    xstring(xyl(1)+r*cos(a),xyl(2)-r*sin(a),Id,ang+180 );
  else
    xstring(xyl(1),xyl(2),Id,ang);
  end
  e3=gce();
  e3.user_data=r
  //if  ~vis then e3.visible='off';end
  
  //make a Compound with the arc elements
  e=glue([e1 e2 e3])
endfunction

