function [rep,stat]=unix_g(cmd)
//unix_g - shell command execution 
//%Syntax
//rep=unix_g(cmd)
//%Parameters
// cmd - a character string
// rep - a column vector of character strings
//%Description
// cmd instruction (sh syntax) is passed to shell, the standard output 
// is redirected  to scilab variable rep.
//%Examples
// unix_g("ls")
//%See also
// host unix_x unix_s
//!
// Copyright INRIA
// Modified by Allan CORNET 2004

  [lhs,rhs]=argn(0)
  if prod(size(cmd))<>1 then error(55,1),end
  
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
    rep=mgetl(tmp)
    if (size(rep,'*')==0) | (length(rep)==0) then
    	rep=[]
    else
     	for k=1:size(rep,'*') 
					rep(k)=oemtochar(rep(k));
			end;
    end
   case -1 then // host failed
    disp('host does not answer...')
    rep=emptystr()
  else
  if MSDOS then 
     	if shl <> 'cmd' then
     		write(%io(2),'unix_g: shell error');
       		rep=emptystr()
     	else
     		msg=mgetl(TMPDIR+'\unix.err')
     		if size(msg,'*')<>0 then
     		  for k=1:size(msg,'*') 
					  msg(k)=oemtochar(msg(k));
				  end
				end
        disp(msg(1))
        rep=emptystr()
     	end
     else 
        msg=mgetl(TMPDIR+'/unix.err')
        if size(msg,'*')<>0 then
     		  for k=1:size(msg,'*') 
					  msg(k)=oemtochar(msg(k));
				  end
				end
        disp(msg(1))
        rep=emptystr()
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
