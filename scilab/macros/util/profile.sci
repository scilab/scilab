function count=profile(fun)
if type(fun)==13 then
  lst=macr2lst(fun)
else
  lst=fun
end
if lst(5)(1)<>'25' then
  error("The function has not been built for profiling")
end
count=get_profile(lst,4)
count=count(1:$-1,:)
count(:,2)=count(:,2)/1000000	
endfunction
