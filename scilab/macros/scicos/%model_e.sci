function varargout=%model_e(i,o)
// Copyright INRIA
//function used only for backward compatibility of scicos blocks gui
  warning('Obsolete use of model(i) in this scicos block')
  varargout=list()
  for k=1:size(i,'*')
    varargout($+1)=getfield(i(k)+1,o)
  end
endfunction
