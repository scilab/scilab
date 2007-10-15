function drawgrid(gh_window)
// Copyright INRIA
//** get the number of right side arguments
   rhs = argn(2) ;

   if rhs==0 then //** without arguments (default) assume ...
       gh_curwin = gcf(); //** get the handle of the current graphics window
    else
       gh_curwin = gh_window ; //** get the handle of the current graphics window
   end

   if ~exists('%scs_wgrid') then
     %scs_wgrid = [10;10;12];
   end
   b1=%scs_wgrid(1) //**x
   b2=%scs_wgrid(2); //**y
   colorr = %scs_wgrid(3) //**color

   gh_axes=gh_curwin.children(1) //** suppose current axes is the first
                                 //** children of figure
   bounds=gh_axes.data_bounds //**[xmin ymin;
                              //** xmax ymax]

   o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object

   //absolute coordinate of grid : always center in [0,0]
   //-Alan- : Please discuss if you want to change this behavior...
   loc_x = (int(bounds(1,1)/b1):1:int(bounds(2,1)/b1))*b1
   loc_y = (int(bounds(1,2)/b2):1:int(bounds(2,2)/b2))*b2

   xpolys([loc_x;
           loc_x],...
          [bounds(1,2)*ones(1,size(loc_x,2));
           bounds(2,2)*ones(1,size(loc_x,2))],...
          colorr*ones(1,size(loc_x,2)))

   xpolys([bounds(1,1)*ones(1,size(loc_y,2));
           bounds(2,1)*ones(1,size(loc_y,2))],...
          [loc_y;
           loc_y],...
          colorr*ones(1,size(loc_y,2)))

   p_size = size ( gh_curwin.children.children ) ; //** size after the draw
   //** aggregate the graphics entities
   d_size =  p_size(1) - o_size(1) ;
   gh_grid = glue( gh_curwin.children.children(d_size:-1:1) ) ;
   if ~%scicos_with_grid(1) then
     gh_grid.visible="off"
   end
endfunction
 
