function vanne_draw_ports(o)
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    xset('pattern',default_color(0))
  // draw input/output ports
  //------------------------

  if orient then  //standard orientation
    // set port shape
    out2=[ 0  -1
	   1  -1
	   1   1
	   0   1]*diag([xf/7,yf/14])
    
    in2= [-1  -1
	   0  -1
	   0   1
	  -1   1]*diag([xf/7,yf/14])
    //dy=sz(2)/2
    xset('pattern',default_color(1))
    //xpoly(out2(:,1)+(orig(1)+sz(1)),..
	//  out2(:,2)+(orig(2)+sz(2)-dy),"lines",1)
    xpoly(out2(:,1)+(orig(1)+sz(1)),..
	  out2(:,2)+(orig(2)+2*sz(2)/10),"lines",1)

    //dy=sz(2)/2
    //xfpoly(in2(:,1)+orig(1),..
	//   in2(:,2)+(orig(2)+sz(2)-dy),1)
   
    xfpoly(in2(:,1)+orig(1),..
	   in2(:,2)+(orig(2)+2*sz(2)/10),1)	
  else //tilded orientation
      out2=[0  -1
	   -1  -1
	   -1   1
	    0   1]*diag([xf/7,yf/14])
      
      in2= [1  -1
	    0  -1
	    0   1
	    1   1]*diag([xf/7,yf/14])

      
      //dy=sz(2)/2
      xset('pattern',default_color(1))
      //xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	//    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),"lines",1)  
      //dy=sz(2)/2
      //xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	//     in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1) 
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	    out2(:,2)+ones(4,1)*(orig(2)+2*sz(2)/10),"lines",1)  
      dy=sz(2)/2
      xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	     in2(:,2)+ones(4,1)*(orig(2)+2*sz(2)/10),1) 
  end
  // valve command port port
  //------------------------
  // set port shape



  in= [-1  1
        0  0
        1  1
       -1  1]*diag([xf/14,yf/7])



  dx=sz(1)/2

  xfpoly(in(:,1)+ones(4,1)*(orig(1)+dx),..
	 in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)

endfunction 
