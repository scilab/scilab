function do_details(x)
// Copyright INRIA

  if type(x)==1 then
    Select=x
    sel_items = size(Select)   ; 
    obj_selected = sel_items(1) ; 
    if obj_selected==0 then
      o=scs_m;
    else
      cwin=Select(1,2)
      if cwin==curwin then
	k = Select(1,1)
	o = scs_m.objs(k)
      elseif or(windows(find(windows(:,1)<0),2)==cwin) then
	kwin = find(windows(:,2)==cwin)   ;
	pal  = palettes(-windows(kwin,1)) ;
	k = Select(1,1) ;
	o = pal.objs(k);
      end
    end
  else
    o=x
  end

  tree_show(o) ; //** beware to the BWidget version 

endfunction
