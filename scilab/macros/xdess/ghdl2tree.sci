function [t] = ghdl2tree(h)
//  converts a graphic handle into a scilab tree (resursive tlist
//Author S. Steer May 2005, Copyright INRIA
  typ=h.type

  //get the vector of fields names and make it a row
  fields=matrix(ghdl_fields(h),1,-1)
 
  if fields<>[] then
    t=tlist([typ,fields])
    for name=fields
      execstr('p=h.'+name)
      if type(p)==9 then // current field is a handle or a vector of handle
	if size(p,'*')== 1 then 
	  p=ghdl2tree(p),
	else
	  chl=list();for k=1:size(p,'*'),chl(k)=ghdl2tree(p(k)),end
	  p=chl
	end
      end
      t($+1)=p
    end
  else //no fields defined
    t=tlist(typ)
  end
endfunction

