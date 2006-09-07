function xs2eps(win_num,filename,colored,orientation)
  [lhs,rhs]=argn(0);
  
  //Input arguments checking
  if rhs<3 then
    orientation='portrait'
    colored=-1
  elseif rhs==3 then
    if or(type(colored)==[1 4]) then
      orientation='portrait'
    elseif type(colored)==10 then
      orientation=colored
      colored=-1
    else
      error('Third argument should be the orientation or the colored flag')
    end
  elseif rhs==4 then
    if ~or(type(colored)==[1 4 10]) then
      error('Third argument should be the orientation or the colored flag')
    end
    if type(colored)==10 then
      [orientation,colored]=(colored,orientation)
    end
  end
  
  if and(type(colored)<>[1 4]) then
    error('the colored argument should be an integer or a boolean')
  end
  if type(orientation)<>10 then
    error('the orientation argument should be a character string')
  end
  
  opt='-'+part(stripblanks(orientation),1)
  if and(opt<>['-p','-l']) then 
    error('orientation should be ""p"" or ""l"" ')
  end
  
  //create the postscript file (without header)
  if colored==-1 then
    xs2ps(win_num,filename)
  else
    xs2ps(win_num,filename,bool2s(colored));
  end
  //add the Postscript file header and create <filename>.eps file
  if MSDOS then
    fname=pathconvert(filename,%f,%t,'w')
    comm=pathconvert(SCI+'\bin\BEpsf',%f,%f,'w')
    rep=unix_g(comm+' '+opt+' '+'""'+fname+'""')
  else
    rep=unix_g(SCI+'/bin/BEpsf '+opt+' '+filename)
  end
  if rep<>[] then 
    error('Problem generating file , perhaps directory is not writable')
  end
endfunction
