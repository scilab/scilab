function graphics=scicos_graphics(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
//initialisation de graphics mlist
  if exists('orig','local')==0 then orig=[0 0],end
  if exists('sz','local')==0 then sz=[20 20],end
  if exists('flip','local')==0 then flip=%t,end
  if exists('exprs','local')==0 then exprs=[],end
  if exists('pin','local')==0 then pin=[],end
  if exists('pout','local')==0 then pout=[],end
  if exists('pein','local')==0 then pein=[],end
  if exists('peout','local')==0 then peout=[],end
  if exists('gr_i','local')==0 then gr_i=[],end
  if exists('id','local')==0 then id='',end
  if exists('in_implicit','local')==0 then 
    I='E';
    in_implicit=I(ones(pin(:)));
  end
  if exists('out_implicit','local')==0 then 
    I='E';
    out_implicit=I(ones(pout(:)));
  end 
  graphics=mlist(['graphics','orig','sz','flip','exprs','pin',..
		  'pout','pein','peout','gr_i','id','in_implicit','out_implicit'],..
		 orig,sz,flip,exprs,pin,pout,pein,peout,gr_i,id,in_implicit,out_implicit)
endfunction
