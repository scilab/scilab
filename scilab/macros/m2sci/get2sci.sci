function [stk,top,vnms,vtps,txt]=get2sci(nam,stk,top,vnms,vtps)
// Translate the named variable acquisition
//!
// Copyright INRIA
txt=[]
top=top+1
vn=find(nam==vnms(:,2))
if vn==[] then // variable is not defined yet
  if ilst+1<=size(lst) then
    op1=lst(ilst+1)
  else
    op1=['?','?']
  end
  std=%f
  if size(op1,'*')>=2 then
    std=op1(1)=='5'&op1(2)=='24'
  end
  if ~std then
    if nam=='nargin' then
      stk(top)=list('nargin','0','1','1','1')
      nv=size(vnms,1);
      vnms=[vnms;['nargin','nargin']]
      vtps(nv+1)=list('1','1','1',0)
    elseif nam=='nargout' then
      stk(top)=list('nargout','0','1','1','1')
      nv=size(vnms,1);
      vnms=[vnms;['nargout','nargout']]
      vtps(nv+1)=list('1','1','1',0)
    else  // undefined variable may be a m_file or global variable
      // check if a corresponding m_file exist here
      if isanmfile(nam) then
	// a m_file without parameter
	stk(top)=list()
      elseif exists('sci_'+nam)==1 then
	// a translated function without parameter
	stk(top)=list()
      else
	[vnms,vtps,stk,txt]=get_unknown(nam)
      end
    end
  else
    stk(top)=list(nam,'0','0','0','?',0)
    vnms=[vnms;[nam,nam]]
    vtps($+1)=list('1','0','0')
  end
else
  vn=vn($)//??????
  stk(top)=list(vnms(vn,1),'0',vtps(vn)(2),vtps(vn)(3),vtps(vn)(1))
end
endfunction
