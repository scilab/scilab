function r=%cell_f_cell(a,b)
da=a.dims;if size(da,'*')<2 then da(2)=1,end
db=b.dims;if size(db,'*')<2 then db(2)=1,end
if size(da,'*')<>size(db,'*') then
  error('In concatenation the number of dimensions for each component must match');
end

kd=find(da<>db)
kd(find(kd==1))=[]
if kd<>[] then error(6);end

val=a.entries
for k=1:prod(db)
  val($+1)=b.entries(k)
end
da(1)=da(1)+db(1)
r=mlist(['cell','dims','entries'],da,val)
endfunction



