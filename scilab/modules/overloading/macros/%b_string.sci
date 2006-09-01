function s=%b_string(b)
  s=emptystr(size(b,1),size(b,2))
  for i=find(b); s(i)='T'; end
  for i=find(~b); s(i)='F'; end
endfunction
