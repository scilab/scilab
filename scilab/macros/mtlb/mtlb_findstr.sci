function s=mtlb_findstr(a,b)
if length(a)>length(b) then
  s=strindex(a,b)
else
  s=strindex(b,a)
end
 
endfunction
