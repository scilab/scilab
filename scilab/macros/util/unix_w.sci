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
// Modified by Allan CORNET 2004-2005
  if prod(size(cmd))<>1 then   error(55,1),end
  
  ver=OS_Version();
  stat=0;
  
  if MSDOS then 
    tmp=strsubst(TMPDIR,'/','\')+'\unix.out';
    if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
    	cmd1= cmd + ' > '+ tmp;
    	stat=host(cmd1);
    else
      // Use 'dos' for 2k and more 
    	tmp=TMPDIR+'\unix.out';
     	[status,ouput]=dos(cmd);
     	if (status == %t) then
     	  mputl(ouput,tmp);
     	  stat=0;
     	else
     	  mputl(ouput,TMPDIR+'\unix.err');
     	  stat=1;
     	end
    end
  else 
     tmp=TMPDIR+'/unix.out';
     cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
     stat=host(cmd1);
  end 
  
  select stat
   case 0 then
   	if MSDOS then
   	  rep=mgetl(tmp);
   	  if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
   	    if size(rep,'*')<>0 then
          for k=1:size(rep,'*') 
			      rep(k)=oemtochar(rep(k));
		      end
		    end
		  end  
      mputl(rep,tmp);
    end
    write(%io(2),read(tmp,-1,1,'(a)'))
   case -1 then // host failed
    error(85)
  else
     if MSDOS then 
     	if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
     		error('unix_w: shell error');
     	else
     		msg=read(TMPDIR+'\unix.err',-1,1,'(a)')
     		error('unix_w: '+msg(1))
     	end
     else
	msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	error('unix_w: '+msg(1))
     end 
  end
  if MSDOS then
    if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
    	host('if exist '+tmp+' del '+tmp);
    else
    	dos('if exist '+tmp+' del '+tmp);
    	dos('if exist '+TMPDIR+'\unix.err'+' del '+TMPDIR+'\unix.err');
    end
  else
     host('rm -f '+tmp);
  end
endfunction
