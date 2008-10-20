function whos(%_opt,%_sel)
//display defined variable in a long form
// Copyright INRIA

//get the variables
  [%_nams,%_vol]=who('get');
  [%_lhs,%_rhs]=argn();
  if %_rhs==1 then error('whos requires zeros or two arguments'),end
  if %_rhs==2 then 
    %_nams($-1:$)=[],
    if %_opt<>'-type'&%_opt<>'-name' then 
      error('First argument must be ""-name"" or ""-type""')
    end
  end
  //write the display header
  write(%io(2),part('Name',1:25)+..
	part('Type',1:15)+..
	part('Size',1:15)+..
	part('Bytes',1:13))
  write(%io(2),' ')

  //loop on variables
  for %_k=1:size(%_nams,1)
    %_sz=' '
    if execstr('%_typ=type('+%_nams(%_k)+')','errcatch')==228 then
      %_typ=0
      %_typn='*deleted'
    else 
      execstr('%_typn=typeof('+%_nams(%_k)+')')
      execstr('%_isg=isglobal('+%_nams(%_k)+')')
      if %_isg then %_typn='*'+%_typn,end
    end
    %_ok=%t 
    if %_rhs==2 then
      if %_opt=='-type' then
	if %_typn<>%_sel then %_ok=%f,end
      elseif %_opt=='-name' then
	if part(%_nams(%_k),1:length(%_sel))<>%_sel then %_ok=%f,end
      end
    end
    
    if %_ok then
      if  %_typ==0 then
	%_sz='?'
      elseif %_typ<=10 then 
	execstr('%_sz=size('+%_nams(%_k)+');')
      elseif %_typ==17|%_typ==16 then
	  execstr('%_tt=getfield(1,'+%_nams(%_k)+')');%_tt=%_tt(1)
	  ierr=execstr('%_sz=%'+%_tt+'_size('+%_nams(%_k)+')','errcatch')
	  if ierr<>0 then %_sz='?',end
      end

      write(%io(2),part(%_nams(%_k),1:25)+part(%_typn,1:15)+..
	    part(strcat(string(%_sz),' by '),1:15)+..
	    part(string(%_vol(%_k)*8),1:13))
    end
  end
endfunction
