function var=%type_i_variable(field,inslist,var)
// Copyright INRIA

if typeof(inslist)=="ce" then
  if field=="type" then
    var.infer=inslist
  else
    error("Not yet implemented...")
  end
elseif typeof(inslist)=="st" then
  pause
else
  if or(typeof(var.infer)==["ce","st"]) then
    if field=="type" then
      dims=list()
      for kd=1:size(var.infer.dims,"*")
	dims(kd)=var.infer.dims(kd)
      end
      var.infer=Infer(dims,Type())
      var.infer.type=inslist
    else
      error("Not yet implemented...")
    end
  else 
    if field=="type" then
      var.infer.type=inslist
    else
      error("Not yet implemented...")
    end
  end
end
endfunction
