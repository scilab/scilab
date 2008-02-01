function %egraphic_p(graphics)
//Overloads edge graphic data structure display
//Author: Serge Steer; Copyright INRIA
  F =['name','width','foreground','font','profile_index']
  w=format_struct(graphics,F)+'|'

  mprintf("  %s\n",w) 

endfunction

