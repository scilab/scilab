//adjust_in2out2
//Alan
//adjust in2,intyp,out2, outtyp in accordance to
//in out (for compatibility)
function [ok,bllst]=adjust_in2out2(bllst)
 ok=%t
 nblk=size(bllst);
 //Check dimension of vectors in/in2,out/out2 
 //and intyp/outyp for each block
 for i=1:nblk
  //input port
  sz_in=size(bllst(i).in,'*');
  sz_in2=size(bllst(i).in2,'*');
  sz_intyp=size(bllst(i).intyp,'*');
  //adjust dimension of in2
  if sz_in<>sz_in2 then
    //typical case : get dimension 1 for in2
    if sz_in2==0 then bllst(i).in2=ones(sz_in,1), end
  end
  //adjust dimension of intyp
  if sz_intyp<sz_in then
    //typical case : get type double for intyp
    if sz_intyp==1 then bllst(i).intyp=ones(sz_in,1), end
  end

  //output port
  sz_out=size(bllst(i).out,'*');
  sz_out2=size(bllst(i).out2,'*');
  sz_outtyp=size(bllst(i).outtyp,'*');
  //adjust dimension of out2
  if sz_out<>sz_out2 then
    //typical case : get dimension 1 for in2
    if sz_out2==0 then bllst(i).out2=ones(sz_out,1), end
  end
  //adjust dimension of outtyp
  if sz_outtyp<sz_out then
    //typical case : get type double for outtyp
    if sz_outtyp==1 then bllst(i).outtyp=ones(sz_out,1), end
  end
 end
endfunction
