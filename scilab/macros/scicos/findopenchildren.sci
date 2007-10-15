function supers=findopenchildren(scs_m,super_path,supers)
// Copyright INRIA
//find paths to all open (inactive) sub-diagrams

  if argn(2)<3 then supers=list(),end
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super' then
        n=size(inactive_windows(1))
        for i=1:n
          path=inactive_windows(1)(i)
          if isequal(path,[super_path,k]) & or(winsid()==inactive_windows(2)(i)) then
             supers($+1)=[super_path,k]
             break
          end
        end
        supers=findopenchildren(model.rpar,[super_path,k],supers)
      end
    end
  end
endfunction
