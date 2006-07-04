function [lnksz,lnktyp]=lnkptrcomp(bllst,inpptr,outptr,inplnk,outlnk)
// Copyright INRIA

lnkbsz=[];ptlnk=[];lnksz=[];lnktyp=[];
for blkout=1:length(bllst)
  for portout=1:outptr(blkout+1)-outptr(blkout)
    ptlnk=outlnk(outptr(blkout)+portout-1)
    lnkbsz(outlnk(outptr(blkout)+portout-1))=bllst(blkout).out(portout);
    lnksz(ptlnk,1)=bllst(blkout).out(portout);
    lnksz(ptlnk,2)=bllst(blkout).out2(portout);
    lnktyp(ptlnk)=bllst(blkout).outtyp(portout);
  end	
end
//lnkptr=cumsum([1;lnkbsz])
endfunction
