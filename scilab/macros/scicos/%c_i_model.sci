function m=%c_i_model(i,b,m)
// Copyright INRIA
//function used only for backward compatibility of scicos blocks gui
  if i>13 then warning('Field '+string(i)+' no longer in model, check'),end
  setfield(i+1,b,m)
endfunction
