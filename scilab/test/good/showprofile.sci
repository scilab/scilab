function showprofile(fun)
lst=macr2lst(fun)
count=profile(lst)
count(:,2)=round(100*count(:,2))/100
count=string(count)

txt=fun2string(lst,'fun')
m=min(size(count,1),size(txt,1))
txt=txt(1:m);count=count(1:m,:)
txt=part(txt,1:max(length(txt)))
for k=1:3
  txt=txt+'|'+part(count(:,k),1:max(length(count(:,k))))
end
txt=txt+'|'
write(%io(2),txt,'(a)')
endfunction
