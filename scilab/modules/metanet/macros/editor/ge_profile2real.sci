
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xyp,xya,xyl,ang]=ge_profile2real(n)
// this function returns the cordinates of the polyline to be used to
// plot the arc.
  arrowWidth=4;arrowLength=6; ArcDx=0.25;ArcDy=15;//a mettre en parametres
  
  //get the arc profile
  xyp=ge_get_edge_profile(n)
  
  tail=GraphList.edges.tail(n);
  c1x=GraphList.nodes.graphics.x(tail)
  c1y=GraphList.nodes.graphics.y(tail)
  r1=ge_get_node_diam(tail)/2
  
  if tail==GraphList.edges.head(n) then //loop arc
    k=find(GraphList.edges.head==tail&GraphList.edges.tail==tail)
    v=(size(find(k<=n),'*')+1)/2
    a=1.2;
    //compute the angle ti of the intersection point between
    //the node circle and arc ellipsis
    // x=r1*cos(t1)    + c1x; y=r1*sin(t1)  + c1y         the circle   
    // x=v*r1*a*cos(t) + c1x; y=v*r1*sin(t) + c1y + v*r1  the ellipse
    // by elimination : cos(t1)=v*a*cos(t); sin(t1)=v*sin(t) + v
    // squaring: 1=v^2*a^2*cos(t)^2+v^2*(sin(t) + 1)^2
    // s=sin(t): v^2*a^2*(1-s^2)+v^2*(s + 1)^2-1=0
    a2=a^2;
    ti=asin(-(v-sqrt(a2^2*v^2-a2+1))/(v*(1-a2)))
    //arc length
    al=(%pi-2*ti)*(v*r1*a)
    //arc path
    t=linspace(ti,%pi-ti,round(al))';
    xyp=[c1x+v*r1*a*cos(t) c1y+v*r1+v*r1*sin(t)];
    //arrow
    xya=[c1x+[0;arrowLength;0] c1y+2*(v*r1)+[arrowWidth;0;-arrowWidth]]
    //Id position
    xyl=[c1x c1y+2*(v*r1)]
    ang=0
  else
    if xyp==[] then xyp=ge_default_edge_profile(),end //compat
    [mx,kmax]=max(xyp(:,2))
    head=GraphList.edges.head(n);
    k=find((GraphList.edges.tail==tail& GraphList.edges.head==head)| ...
	   (GraphList.edges.head==tail& GraphList.edges.tail==head))
    v=size(find(k<n),'*')
    epai=((-1).^v).*round(v/2).*sign(head-tail);

    c2x=GraphList.nodes.graphics.x(head)
    c2y=GraphList.nodes.graphics.y(head)
    
    r2=ge_get_node_diam(head)/2;
    l=sqrt((c2x-c1x)^2+(c2y-c1y)^2); //distance between the node//centers
    if l>r1+r2 then
      //arc baseline rotation
      co=(c2x-c1x)/l;si=-(c2y-c1y)/l;
    else
      co=0;si=1;
    end

    l1=(l-r1-r2); //arc baseline length
    
    //translate and dilate profile
    xyp=[r1+l1*xyp(:,1),  xyp(:,2)*(epai*ArcDy)]

    //rotate profile and translate again
    xyp=ones(size(xyp,1),1)*[c1x c1y]+xyp*[co -si;si co]
    ang=180+180*atan(si,co)/%pi
    xym=xyp(kmax,:)
    
    //arrow profile
    xya=[0            arrowWidth;
	 arrowLength  0         ;
	 0           -arrowWidth];
    //rotate and translate
    xya=ones(3,1)*xym+xya*[co -si;si co]
    xyl=xym+ArcDy/10*[-si co]
  end
endfunction

