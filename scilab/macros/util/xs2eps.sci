function xs2eps(win_num,filename,color)
  [lhs,rhs]=argn(0);
  if rhs <=2 then 
    xs2ps(win_num,filename);
  else
    xs2ps(win_num,filename,color);
  end
  //add the Postscript file header and create <filename>.eps file
  opt=" -p "
  if MSDOS then
    fname=pathconvert(fileame,%f,%t,'w')
    comm=pathconvert(SCI+'\bin\BEpsf',%f,%f,'w')
    rep=unix_g(comm+' '+opt+'""'+fname+'""')
  else
    rep=unix_g(SCI+'/bin/BEpsf '+opt+filename)
    //unix_s(pathconvert('SCI/bin/BEpsf',%f)+op +filename)
  end
  if rep<>[] then 
    message(['Problem generating ps file.';..
	     'perhaps directory not writable'] )
  end
endfunction
