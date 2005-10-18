function scicos_pal=update_scicos_pal(path,name,fname)
// Copyright INRIA
  scicos_pal;

  inde=find(scicos_pal(:,1)==name);
  if size(inde,'*')>=2 then 
    message(['More than one palette named '+name;
	     'This is not allowed, do an Pal Editor to correct'])
    return
  end
  if inde<>[] then
    if message(['The palette '+name+' already exists';
		   'Do you want to replace it?'],['Yes','No'])==2 then 
      return;
    else
      scicos_pal(inde,2)=fname
      if MSDOS then 
	instr='del '+TMPDIR+'\'+name+'.pal'
      else
	instr='\rm -f '+TMPDIR+'/'+name+'.pal'
      end
      if execstr('unix_s(instr)','errcatch')<>0 then
	x_message(['I was not able to delete '+name+'.pal';
		   'in '+TMPDIR+'. You must do it now!'])
      end
    end
  else
    scicos_pal=[scicos_pal;[name,fname]]
  end
  if execstr('save(''.scicos_pal'',scicos_pal)','errcatch')<>0 then
    x_message(['I was not able to write in .scicos_pal:';lasterror()])
  end
endfunction
