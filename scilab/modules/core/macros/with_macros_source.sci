function r=with_macros_source()
  res = ls(SCI+"/modules/core/macros/*.sci");
  if res <> [] then
    r=%T;
  else
    r=%F;
  end
endfunction
