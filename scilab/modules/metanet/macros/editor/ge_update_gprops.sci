function ge_update_gprops(e,propname,propvalue)
  for k=1:size(e,'*')
    if e(k).type=="Compound" then
      n=size(e(k).children,'*')
      for i=1:n
	set(e(k).children(i),propname,propvalue)
      end
    else
      set(e(k),propname,propvalue)
    end
  end
endfunction
