function [k,wh,scs_m] = stupid_getobj(scs_m,Select,pt)
// Copyright INRIA

//** Now works only for a selected link
  
  n = lstsize(scs_m.objs)
  wh = [];
  x = pt(1); y = pt(2) ; //** mouse coordinate 
  data = [] ;
  k = [];

  i=Select(1)  // Link to move
  o = scs_m.objs(i);
  
  
    eps = 3 ;
    xx = o.xx; yy = o.yy;
    [d, ptp, ind] = stupid_dist2polyline(xx, yy, pt, 0.85)
    
    if d<eps then 
      
      if ind==-1 then 
	k = o.from(1); //** click near an input
      elseif ind==-size(xx,1) then 
	k = o.to(1) ; //** click near an output
      elseif ind>0 then 
	// draw_link_seg(o,[ind,ind+1]) //** debug only
	o.xx = [xx(1:ind);ptp(1);xx(ind+1:$)];
	o.yy = [yy(1:ind);ptp(2);yy(ind+1:$)];
	scs_m.objs(i) = o ; //** CESTICIQUONCREEUNPOINTDANSSCS_M
			    //** ETQUONOUBLIEDELEFAIREDANSLEGRAPHIQUE !!
	k  = i ;
	wh = -ind - 1 ; //** click in the middle (case 1) of a link
      else
	k = i
	wh = ind      ; //** click in the middle (case 2) of a link
			// draw_link_seg(o,[-ind-1:-ind+1]); //** debug only
      end
    end
endfunction
