function tklib=gettklib()
	if MSDOS then 
		tklib='tk84.dll'
	else 
		// If libtk8.4.so (or .sl) exists in SCI/bin => it's ok
		if fileinfo('SCI/bin/libtk8.4.so') <> [] then
			tklib='libtk8.4.so';
			return;
		end
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
