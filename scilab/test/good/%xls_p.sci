function %xls_p(sheets)
  //displays excel set of sheets
  S=sheets.sheets
  for k=1:size(S)
    V=S(k).value
    name=S(k).name
    mprintf("  %s: %dx%d\n",name,size(V,1),size(V,2))
  end
endfunction
