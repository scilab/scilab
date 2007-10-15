function mo=modelica(v1,v2,v3,v4)
// Copyright INRIA

  if exists('model','local')==0 then model=[], end
  if exists('inputs','local')==0 then inputs=[], end
  if exists('outputs','local')==0 then outputs=[], end
  if exists('parameters','local')==0 then parameters=list([],list()), end

  mo=tlist(['modelica','model','inputs','outputs','parameters'],...
            model,inputs,outputs,parameters)
endfunction
