function p=metanet_module_path()
  //returns the path where the metanet toolbox is located
  execstr('w=string('+whereis('metanet_module_path')+')')
  p=w(1)
  k=strindex(p,['/' '\'])
  p=part(p,1:k($-2))
endfunction
