function Move_()
// Copyright INRIA

  Cmenu = []; //** NB : preserve %pt information 

  SelectSize = size (Select) ; //** [row, col]
  SelectSize = SelectSize(1) ; //**  row
  if Select<>[] then
     if find(Select(:,2)<>curwin)<>[] then
       Select=[]
       Cmenu = 'Move'
       return
     end
  end

  if  SelectSize == 1 & typeof(scs_m.objs(Select(1)))=="Link" then
  //** if ONE or NO object are selected 
     [scs_m] = do_stupidmove(%pt, Select,scs_m) ; //** Stupid is whos stupid does :)
                                           //** Select is not used here ....
  else //** multiple object are selected

      [scs_m] = do_stupidMultimove(%pt, Select, scs_m) ; //** move multiple objects
                //** %pt    : last valid user LEFT BUTTON PRESS
		//** Select : matrix of selected object
		//**  Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
		//**                                  and "win_id"    is the Scilab window id.
		//**  Multiple selection is permitted: each object is a line of the matrix.
		//** scs_m  : diagram datastucture
  end

  %pt = [];

endfunction
