function %c_p(a)
texout(a)
  return
  t=''
  for k=1:size(a,2)
    t=t+part(a(:,k),1:max(length(a(:,k)))+1)
  end
  write(%io(2),t,'(a)')
endfunction
