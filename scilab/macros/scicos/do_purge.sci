function scs_m_new=do_purge(scs_m)
// Copyright INRIA

//suppress deleted elements in a scicos data structure
  nx=size(scs_m);


  //get index of deleted blocks
  deleted=[];
  for k=1:nx
    typ=typeof(scs_m(k))
    if typ=='Deleted' then
      deleted=[deleted,k];
    elseif typ=='Block' then
      if scs_m(k).model.sim=='super' then
	scs_m(k).model.rpar=do_purge(scs_m(k).model.rpar)
      end
    end
  end

  if deleted==[] then //nothing has to be done
    scs_m_new=scs_m
    return
  end

  retained=1:nx;retained(deleted)=[];
  //compute index cross table
  old_to_new=ones(1,nx);old_to_new(deleted)=0*deleted;
  old_to_new=rtitr(1,%z-1,old_to_new)';//old_to_new(1)=[];

  scs_m_new=list();
  for k=1:size(retained,'*')
    o=scs_m(retained(k))
    if typeof(o)=='Block' then
      if o.graphics.pin<>[] then
	o.graphics.pin=old_to_new(o.graphics.pin+1);
      end
      if o.graphics.pout<>[] then
	o.graphics.pout=old_to_new(o.graphics.pout+1);
      end
      if o.graphics.pein<>[] then
	o.graphics.pein=old_to_new(o.graphics.pein+1);
      end
      if o.graphics.peout<>[] then
	o.graphics.peout=old_to_new(o.graphics.peout+1);
      end
    elseif typeof(o)=='Link' then
      o.from(1)=old_to_new(o.from(1)+1);
      o.to(1)=old_to_new(o.to(1)+1);
    end
    scs_m_new(k)=o;
  end
endfunction
