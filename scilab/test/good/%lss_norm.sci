function y=%lss_norm(A,flag)
  if argn(2)==1 then flag=2,end
  if flag==2 then
    y=h2norm(A)
  elseif flag==%inf|flag=='inf' then
    y=h_norm(A)
  else
    error('flag must be 2 or inf.')
  end
endfunction
