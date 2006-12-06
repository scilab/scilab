function tklib=gettklib()
  if MSDOS then 
    tklib='tk84.dll'
  else 
    OS=getos();
    localelibnametk=SCI+'/bin/libtk8.4.so';
    if (OS != 'HP-UX') then
	    valret=execstr('link(localelibnametk)', 'errcatch')
	  else
	    valret=0;
	  end
	  if (valret == 0) then
	    tklib=localelibnametk
	  else
		  localelibnametk=SCI+'/bin/libtk8.4.so.0';
		  if (OS != 'HP-UX') then
	      valret=execstr('link(localelibnametk)', 'errcatch')
	    else
	      valret=0;
	    end
		  if (valret == 0) then
		    tklib=localelibnametk
		  else
			  mprintf('Warning: Error loading libtk8.4.so :""'+lasterror()+'""')
		  end
	  end
  end
endfunction
