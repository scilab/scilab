function rep=ilib_unix_soname()
// Copyright Enpc 
// try to get the proper sufix for 
// shared unix library sl or so ? 
  libtoolconf=unix_g("$SCI/libtool --config")
  //check if shared libraries are built
  k=grep(libtoolconf,'build_libtool_libs')
  if strindex(libtoolconf(k),'yes')<>[] then //yes
     //k=grep(libtoolconf,'soname_spec')
     k=grep(libtoolconf,'shrext=')
     if strindex(libtoolconf(k),".sl")<>[] 
       rep="sl" ;
     else 
       rep="so" ; 
     end 
  else //no shared library built
     rep='a'
  end
endfunction
