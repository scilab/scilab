
function o=%c_i_block(i,b,o)
//function used only for backward compatibility of scicos blocks gui
  if i<>1 then o=setfield(i,b,o),end
endfunction
