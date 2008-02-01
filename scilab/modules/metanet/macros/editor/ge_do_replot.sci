function ge_do_replot(GraphList)
  ge_disablemenus()
  clf()
  ge_set_winsize()
  ge_axes_handle=gca()
  
  set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))
  ge_draw_graph(GraphList)
  ge_enablemenus();

endfunction
