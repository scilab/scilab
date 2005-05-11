function xs2eps(win_num,filename,color)
  [lhs,rhs]=argn(0);
  if rhs <=2 then 
    xs2ps(win_num,filename);
  else
    xs2ps(win_num,filename,color);
  end
  //add the Postscript file header and create <filename>.eps file
  if MSDOS then
    unix_s(pathconvert('""'+WSCI+'/bin/BEpsf'+'""',%f)+' -p ' +filename)
  else
    unix_s(pathconvert('SCI/bin/BEpsf',%f)+' -p ' +filename)
  end
endfunction
