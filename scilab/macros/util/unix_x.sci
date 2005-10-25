function unix_x(cmd)
//unix_x - shell command execution, results redirected in a window
//%Syntax
// unix_x(cmd)
//%Parameters
// cmd - a character string
//%Description
// cmd instruction is passed to shell, the standard output is redirected 
// to  a  window
//%Examples
// unix_x("ls")
//%See also
// host unix_g unix_s
//!
// Copyright INRIA
// Modified by Allan CORNET 2004

if prod(size(cmd))<>1 then   error(55,1),end

ver=OS_Version();
stat=0;

if MSDOS then 
    tmp=strsubst(TMPDIR,'/','\')+'\unix.out';
    if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
    	cmd1= cmd + ' > '+ tmp;
    	stat=host(cmd1);
    else
    	tmp=TMPDIR+'\unix.out';
    	// Use 'dos' for 2k and more 
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
  	rep=mgetl(tmp)
    if size(rep,'*')==0 then
    	rep=[]
    else
      if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
     	  for k=1:size(rep,'*') 
					rep(k)=oemtochar(rep(k));
			  end;
			end
    end
    x_message_modeless(rep)
case -1 then // host failed
  error(85)
else //sh failed
if MSDOS then 
     	if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
     		error('unix_x: shell error');
     	else
     		msg=read(TMPDIR+'\unix.err',-1,1,'(a)')
     		if size(msg,'*')==0 then
    	    msg=[]
        else
          if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
     	      for k=1:size(msg,'*') 
					    msg(k)=oemtochar(msg(k));
			      end;
			    end;
        end
     		error('unix_x: '+msg(1))
     	end
     else
	msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	error('unix_x: '+msg(1))
     end 
end
endfunction
