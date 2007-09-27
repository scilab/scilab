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

//27/09/07 Alan's patch : do test here for unconnected inputs
unco=find(inplnk>max(outlnk))
for j=unco
  m=maxi(find(inpptr<=j))
  n=j-inpptr(m)+1
  lnksz($+1,1)=bllst(m).in(n);
  lnksz($,2)=bllst(m).in2(n);
  lnktyp($+1)=bllst(m).intyp(n);
end

//lnkptr=cumsum([1;lnkbsz])
endfunction
