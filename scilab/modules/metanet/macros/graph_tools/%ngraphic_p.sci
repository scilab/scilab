function %ngraphic_p(graphics)
//Overloads node graphic data structure display
//Author: Serge Steer; Copyright INRIA
  F =['name','x','y','type','diam','border','colors','font']
  w=format_struct(graphics,F)+'|'

  mprintf("  %s\n",w) 

endfunction

