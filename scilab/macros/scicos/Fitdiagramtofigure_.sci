function Fitdiagramtofigure_()
// Copyright INRIA
//** Alan-21/12/06 : Fit diagram to figure (Based on new graphics)
   Cmenu=[];
   xinfo('Fit diagram to figure');
   gh_curwin = gh_current_window; //** get the handle of the current graphics window
   r = gh_curwin.figure_size;     //** acquire the current figure physical size
   rect = dig_bound(scs_m);       //** Scicos diagram size
   if rect==[] then               //** if the schematics is not defined
     return;                      //**   then return
   end
   w = (rect(3)-rect(1));
   h = (rect(4)-rect(2));
   margins = [0.02 0.02 0.02 0.02]
   %zoom_w=r(1)/(w*(1+margins(1)+margins(2)))
   %zoom_h=r(2)/(h*(1+margins(3)+margins(4)))

   //** to debug
   if %scicos_debug_gr then
     printf("zoom=%f\n",%zoom);
     printf("zoom_w=%f\n",%zoom_w);
     printf("zoom_h=%f\n",%zoom_h);
   end

   %zoom=min(%zoom_w,%zoom_h);

   gh_window = gcf();             //*get handle of current window

   window_set_size(gh_window);
   if exists('%scicos_with_grid') then
     drawgrid();
     swap_handles(gh_window.children.children($),...
                  gh_window.children.children(1));
     delete(gh_window.children.children(1));
   end
   drawnow();
   show_pixmap();
   xinfo(' ');
endfunction
