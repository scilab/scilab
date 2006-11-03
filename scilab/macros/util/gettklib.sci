function tklib=gettklib()
  if MSDOS then 
    tklib='tk84.dll'
  else 
	execstr('link(''libtk8.4.so'')', 'errcatch')
	if ans == 0 then
	    tklib='libtk8.4.so'
	else
		execstr('link(''libtk8.4.so.0'')', 'errcatch')
		if ans == 0 then
		    tklib='libtk8.4.so.0'
		else
			 mprintf('Warning: Error loading libtk8.4.so :""'+lasterror()+'""')
		end
	end
  end
endfunction
