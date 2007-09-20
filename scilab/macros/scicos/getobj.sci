function [k, wh] = getobj(scs_m, pt)
//** 24/07/07: Al@n's patch for rotation of blocks
//** 26/07/07: Al@n : case for text
// Copyright INRIA

//** ---------- Magic Numbers --------------------------

eps  = 6 ; //** 

//** ---------------------------------------------------

//** init variable with defult value
k    = [] ; //** the index of the object (if found) 
wh   = [] ; //** ?
data = [] ; //** the vector used for object detection 

//** get the number of obj in scs_m
n = lstsize(scs_m.objs)


//** x,y are the last valid "xclick" mouse coord.
x = pt(1);
y = pt(2);


//** disp("...getlink..."); pause 

//** FIRST LOOP on BLOKS only: priority to BLOCKS
//** ... try to fix the SPLIT detection 
for i=n:-1:1
  
  o = scs_m.objs(i);

  if typeof(o)=="Block" then
    //** ------------------- BLOKS --------------------------
      [orig, sz] = (o.graphics.orig, o.graphics.sz) ; //** origin and size
      
      //if scs_m.objs(i).gui=="SPLIT_f" | scs_m.objs(i).gui=="CLKSPLIT_f"
      //  eps = 12  ; //** quad sensivity
      //else
      //  eps =  6  ; //** standard  	
      //end  
      
      sz   = sz + 2*eps ; //** the dection size is larger than the phisical size 
      orig = orig - eps ; //** geometrical correction of the coordinate 
      
      //** geometrical correction for rotated object 
      if %pt<>[] then
        xxx = rotate( [pt(1);pt(2)],...
                      -o.graphics.theta * %pi/180,...
                       [ orig(1) + sz(1)/2 ; orig(2) + sz(2)/2 ]) ; 
        x = xxx(1) ; //** real coordinate used for detection 
        y = xxx(2) ;
      end

      data = [(orig(1)-x)*(orig(1)+sz(1)-x),..
              (orig(2)-y)*(orig(2)+sz(2)-y)]

      //** check if the click is inside the box
      if data(1)<0 & data(2)<0 then
        k = i ; //** take the index 
        return ; //** EXIT Point 
      end

    
   end //** of if()  

end //** end of FIRST for() on all "scs_m" object  

//** SECOND loop on objects
for i=n:-1:1
  o = scs_m.objs(i);

  select typeof(o)

    case "Text" then
    //** ------------------- TEXT --------------------------
      [orig,sz] = (o.graphics.orig,o.graphics.sz)

      //** geometrical correction for rotated text object 
      if %pt<>[] then
        xxx = rotate([pt(1);pt(2)],...
                    -o.graphics.theta*%pi/180,...
                     [ orig(1)+sz(1)/2;orig(2)+sz(2)/2 ] );
        x = xxx(1);
        y = xxx(2);
      end

      data = [(orig(1)-x)*(orig(1)+sz(1)-x),..
              (orig(2)-y)*(orig(2)+sz(2)-y)]

      if data(1)<0 & data(2)<0 then
        k = i  ; //** take the index 
        return  ; //** EXIT Point 
      end

     case "Link" then
     //** ------------------- LINK --------------------------
       xx = o.xx;
       yy = o.yy;
       
       [d, ptp, ind] = dist2polyline(xx, yy, pt) ; 
       
       if d < eps/2 then
         k  = i   ; //** object index  
         wh = ind ; //** ??? ... see "dist2polyline(...)"
         return 
       end
    
   end //** of the switch 

end //** end of for() on all "scs_m" object 



endfunction
