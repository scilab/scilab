function tklib=gettklib()
  if MSDOS then 
    tklib='tk83.dll', 
  else 
    tklib='libtk8.4.so',
  end
endfunction
