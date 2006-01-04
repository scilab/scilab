function unix_w(cmd)
//unix_w - shell command execution results redirected in main scilab window
//%Syntax
// unix_w(cmd)
//%Parameters
// cmd - a character string
//%Description
// cmd instruction (sh syntax) is passed to shell, the standard output 
// is redirected  to main scilab window
//%Examples
// unix_w("ls")
//%See also
// host unix_x unix_s unix_g
//!
// Copyright INRIA
// Modified by Allan CORNET 2004
  if prod(size(cmd))<>1 then   error(55,1),end
  
  shl=getshell();
  
  if MSDOS then 
    tmp=strsubst(TMPDIR,'/','\')+'\unix.out';
    if shl <> 'cmd' then
    	cmd1= cmd + ' > '+ tmp;
    else
    	tmp=TMPDIR+'\unix.out';
     	cmd1=cmd +'>'+ tmp +' 2>'+TMPDIR+'\unix.err';
    end
  else 
     tmp=TMPDIR+'/unix.out';
     cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
  end 
  
  stat=host(cmd1);
  
  select stat
   case 0 then
   	if MSDOS then
   	  rep=mgetl(tmp);
   	  if size(rep,'*')<>0 then
        for k=1:size(rep,'*') 
			    rep(k)=oemtochar(rep(k));
		    end
		  end
      mputl(rep,tmp);
    end
    write(%io(2),read(tmp,-1,1,'(a)'))
   case -1 then // host failed
    error(85)
  else
     if MSDOS then 
     	if shl <> 'cmd' then
     		error('unix_w: shell error');
     	else
     		msg=read(TMPDIR+'\unix.err',-1,1,'(a)')
     		if size(msg,'*')<>0 then
          for k=1:size(msg,'*') 
			      msg(k)=oemtochar(msg(k));
		      end
		  	end
     		error('unix_w: '+msg(1))
     	end
     else
	msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	error('unix_w: '+msg(1))
     end 
  end
  if MSDOS then
    if shl <> 'cmd' then
    	host('if exist '+tmp+' del '+tmp);
    else
    	host('if exist '+tmp+' del '+tmp);
    	host('if exist '+TMPDIR+'\unix.err'+' del '+TMPDIR+'\unix.err');
    end
  else
     host('rm -f '+tmp);
  end
endfunction
