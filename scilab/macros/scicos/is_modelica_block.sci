function r=is_modelica_block(o)
// Copyright INRIA
//checks if the block o is a modelica block
if or(getfield(1,o.model)=='equations') then
  r=o.model.equations<>list()
else
  r=%f
end
endfunction
