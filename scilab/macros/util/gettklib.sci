function tklib=gettklib()
  if MSDOS then 
    tklib='tk84.dll', 
  else 
    tklib='libtk8.4.so',
  end
endfunction
