function tklib=gettklib()
  if MSDOS then 
    tklib='tk84.dll', 
  else 
    tklib='libtk8.3.so'; // 'libtk8.3.so',
  end
endfunction
