function [%vv_list,%ierr_vec]=context_evstr(%str,%scicos_context)
// Copyright INRIA
// Evaluate Scicos Context utility function
 %mm=getfield(1,%scicos_context)
 for %mi=%mm(3:$)
   if execstr(%mi+'=%scicos_context(%mi)','errcatch')<>0 then
     disp(lasterror())
     %ok=%f
     return
   end
 end
 %nn=prod(size(%str))
 %ierr_vec=zeros(%nn,1)
 %vv_list=list()
 for %kk=1:%nn
   [%vv_list(%kk),%ierr_vec(%kk)]=evstr(%str(%kk))
 end
endfunction
