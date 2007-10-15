function [ok,bllst]=adjust_in2out2(bllst)
// Copyright INRIA
 //adjust_in2out2
 //Alan
 //adjust in2,intyp,out2, outtyp in accordance to
 //in out (for compatibility)
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
  if sz_in2<sz_in then
      bllst(i).in2=[bllst(i).in2;ones(sz_in-sz_in2,1)]
  end
  //adjust dimension of intyp
  if sz_intyp<sz_in then
    bllst(i).intyp=[bllst(i).intyp;ones(sz_in-sz_intyp,1)]
  end

  //output port
  sz_out=size(bllst(i).out,'*');
  sz_out2=size(bllst(i).out2,'*');
  sz_outtyp=size(bllst(i).outtyp,'*');
  //adjust dimension of out2
  if sz_out2<sz_out then
       bllst(i).out2=[bllst(i).out2;ones(sz_out-sz_out2,1)]
  end
  //adjust dimension of outtyp
  if sz_outtyp<sz_out then
    bllst(i).outtyp=[bllst(i).outtyp;ones(sz_out-sz_outtyp,1)]
  end
 end
endfunction
