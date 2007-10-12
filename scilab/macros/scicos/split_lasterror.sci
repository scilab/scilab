function str_out=split_lasterror(str_in)
//** Fix the unreadable error messages
//** returned by the simulator.
//** Alan, 11/10/07 : Initial rev
//Copyright INRIA
  str_out=str_in;
  ind_bl=strindex(str_in,' ');
  if find(ind_bl>50)<>[] then
    ind_bl2=ind_bl; nind=[];
    for i=1:size(ind_bl,2)
       if ind_bl2(i)>50 then
         nind=[nind;ind_bl(i)];
         ind_bl2=ind_bl2-ind_bl2(i);
       end
    end
    str_out=strsplit(str_in,nind)
  end
endfunction
