function rep=ilib_unix_soname()
// try to get the proper sufix for 
// shared unix library sl or so ? 
  str=unix_g("$SCI/libtool --config| grep soname_spec ");
  if strindex(str,".sl")<>[] 
    rep="sl" ;
  else 
     rep="so" ; 
  end 
endfunction
