function h=%h_copy(h,ax)
  save(TMPDIR+'/ghdlcopy',h)
  if argn(2)<2 then 
    load(TMPDIR+'/ghdlcopy')
  else
    a=get('current_axes')
    set('current_axes', ax)
    load(TMPDIR+'/ghdlcopy')
    set('current_axes', a)
  end
endfunction
