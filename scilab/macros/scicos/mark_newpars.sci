function newparameters=mark_newpars(k,newparametersb,newparameters)
// k block index in x
//
// Copyright INRIA
o=scs_m(k)
//model=o(3)
model=o.model
//if model(1)=='super'|model(1)=='csuper'
if model.sim=='super'|model.sim=='csuper'
  for npb=newparametersb
    ok=%t;
    for np=newparameters
      if np==[k npb] then
	ok=%f;break,
      end
    end
    if ok then
      newparameters(size(newparameters)+1)=[k npb];
    end
  end
else
  ok=%t
  for np=newparameters
    if np==k then
      ok=%f;break;
    end
  end
  if ok then
    newparameters(size(newparameters)+1)=k
  end
end
endfunction
