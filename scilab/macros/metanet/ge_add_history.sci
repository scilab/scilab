function ge_add_history(op)
//Copyright INRIA
//Author : Serge Steer 2002

  execstr('global EGhist_'+w+';EGhist_'+w+'($+1)=op')
endfunction
