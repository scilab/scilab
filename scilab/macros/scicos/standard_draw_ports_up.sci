function standard_draw_ports_up(o)
// Copyright INRIA

  nin=size(o.model.in,1);

  inporttype=o.graphics.in_implicit
  nout=size(o.model.out,1);
  outporttype=o.graphics.out_implicit
  [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    xset('pattern',default_color(0));

    // set port shape

      out2=[0  -1
	   -1  -1
	   -1   1
	    0   1]*diag([xf/7,yf/14])
      
      in2= [1  -1
	    0  -1
	    0   1
	    1   1]*diag([xf/7,yf/14])

      
      dy=sz(2)/3
      xset('pattern',default_color(1))
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	  out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*2),"lines",1)
      xpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	  in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*2),"lines",1) 
       xfpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	  out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1)
      xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	  in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1)      
   
endfunction 
