function scs_m_new=do_purge(scs_m)
// Copyright INRIA

//suppress deleted elements in a scicos data structure
nx=size(scs_m);


//get index of deleted blocks
deleted=[];
for k=1:nx
 o=scs_m(k);
 if o(1)=='Deleted' then
   deleted=[deleted,k];
 elseif o(1)=='Block' then
   if o(3)(1)=='super' then
     scs_m(k)(3)(8)=do_purge(scs_m(k)(3)(8))
   end
 end
end


if deleted==[] then
  scs_m_new=scs_m
  return
end


retained=1:nx;retained(deleted)=[];
//compute index cross table
old_to_new=ones(1,nx);old_to_new(deleted)=0*deleted;
old_to_new=rtitr(1,%z-1,old_to_new)';//old_to_new(1)=[];
//
scs_m_new=list();
for k=1:size(retained,'*')
  o=scs_m(retained(k))
  if o(1)=='Block' then
    graphics=o(2)
    for i=5:8
      if graphics(i)<>[] then
	graphics(i)=old_to_new(graphics(i)+1);
      end
    end
    o(2)=graphics
  elseif o(1)=='Link' then
    [from,to]=o(8:9)
    from(1)=old_to_new(from(1)+1);
    to(1)=old_to_new(to(1)+1);
    o(8)=from
    o(9)=to
  end
  scs_m_new(k)=o;
end






