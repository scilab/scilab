function r=with_tk()
  [v,opt]=getversion()
  
  r=or(opt=='tk')
  //It should be checked why TK is not enabled with -nw
  r=r&and(sciargs()<>'-nw')
endfunction
