function %diagram_p(scs_m)
// Copyright INRIA
   %params_p(scs_m.props)
   nams=[]
   for o=scs_m.objs
     if typeof(o)=='Block' then
       nams=[nams;o.gui]
     end
   end
   nums=part(string(1:size(nams,'*'))',1:6)
   write(%io(2),nums+nams,'(a)')
endfunction
