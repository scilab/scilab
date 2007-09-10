function count=profile(fun)
if type(fun)==13 then
  lst=macr2lst(fun)
else
  lst=fun
end
count=get_profile(lst,4)
count=count(1:$-1,:)
count(:,2)=count(:,2)/1000000	
endfunction
