function [stk,top,vnms,vtps]=get2f(nam,stk,top,vnms,vtps)
// realise la traduction fortran de l'acquisition d'une variable (stackg)
//!
// Copyright INRIA
top=top+1
vn=find(nam==vnms(:,2))
if vn==[] then
  op1=lst(ilst+1)
  if op1(1)<>'5'|op1(2)<>'24' then
    if nam=='%eps' then
      stk(top)=list('%eps','0','1','1','1')
      nv=prod(size(vnms))/2+1;
      vnms=[vnms;['%eps','%eps']]
      vtps(nv)=list('1','1','1',0)
    elseif nam=='%i' then
      stk(top)=list(['0.0d0','1.0d0'],'0','1','1','1')
    elseif nam=='%io' then
      stk(top)=list(%io,'0','0','1','2')
      nv=prod(size(vnms))/2+1;
      vnms=[vnms;['%io','%io']]
      vtps(nv)=list('0','1','2',0)
    else
      error('variable: ' +nam+' undefined!')
    end
  else
    stk(top)=list(nam,'?','0','0','0',0)
  end
else
  typ=vtps(vn)
//  stk(top)=list(vnms(vn,2),'0',typ(1),typ(2),typ(3))
  stk(top)=list(vnms(vn,2),'0',typ(1),typ(2),typ(3))
end
endfunction
