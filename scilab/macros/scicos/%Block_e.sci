function varargout=%Block_e(i,o)
// Copyright INRIA
//function used only for backward compatibility of scicos blocks gui
  warning('Obsolete use of o(i) in this scicos block')
  varargout=list()
  for k=1:size(i,'*')
    ik=i(k)
    if ik==1 then
      varargout($+1)='Block'
    else
      varargout($+1)=getfield(ik,o)
    end
  end
endfunction
