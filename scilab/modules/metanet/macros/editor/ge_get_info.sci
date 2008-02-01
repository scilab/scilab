function txt=ge_get_info(info)
//Copyright INRIA
//Author : Serge Steer 2002

//This function may be redefined by the user to get a textual
//form of the informations associated with the current diagram
// Copyright INRIA
if size(info)>0 then
  txt=info(1)
else
  txt=[]
end
endfunction
