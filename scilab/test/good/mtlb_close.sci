function [status]=mtlb_close(h)
// Copyright INRIA
// Emulation function for Matlab close()
// V.C.

rhs=argn(2)

warning("mtlb_close: status ignored")
status=1

// close
if rhs<=0 then
  if type(gcf())==9 then
    delete(gcf())
  else
    close()
  end
// close(h) - close('all') - close(name)
elseif rhs==1 then
  if type(h)==9 then 
    delete(h)
  elseif type(h)==1 then 
    close(h)
  elseif type(h)==10 then
    if h=="all" then
      xdel(winsid())
    else // close(name)
      cf_save=gcf()
      
      allwin=winsid()
      for k=1:size(allwin,"*")
	if get(scf(allwin(k)),"figure_name")==h then
	  delete(gcf())
	  break
	end
      end

      scf(cf_save)
    end
  else // Unknown type for h
    error("Not implemented")
  end
else // close('all','hidden')
  warning("mtlb_close: all windows deleted");
  xdel(winsid())
end
endfunction


