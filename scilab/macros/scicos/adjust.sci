function [ok,bllst]=adjust(bllst,inpptr,outptr,inplnk,outlnk)
//
// Copyright INRIA
[outoin,outoinptr]=connmat(inpptr,outptr,inplnk,outlnk)
//
for hh=1:length(bllst)
  ok=%t
  for blkout=1:length(bllst)
    for portout=1:outptr(blkout+1)-outptr(blkout)
      jj=outptr(blkout)+portout-1
      for kk=outoinptr(jj):outoinptr(jj+1)-1
	blkin=outoin(kk,1);portin=outoin(kk,2)
	//
	nout=bllst(blkout)(3)(portout);nin=bllst(blkin)(2)(portin)
	if (nout>0&nin>0) then
	  if nin<>nout then
	    bad_connection(corinv(blkout),portout,nout,corinv(blkin),portin,nin)
	    ok=%f;return
	  end
	elseif (nout>0&nin<0) then 
	  ww=find(bllst(blkin)(2)==nin)
	  if ww<>[] then
	    bllst(blkin)(2)(ww)=nout
	  end
	  ww=find(bllst(blkin)(3)==nin)
	  if ww<>[] then  
	    bllst(blkin)(3)(ww)=nout
	  end
	elseif (nin>0&nout<0) then 
	  ww=find(bllst(blkout)(3)==nout)
	  if ww<>[] then
	    bllst(blkout)(3)(ww)=nin
	  end
	  ww=find(bllst(blkout)(2)==nout)
	  if ww<>[] then
	    bllst(blkout)(2)(ww)=nin
	  end
	elseif (nin==0) then
	  ww=bllst(blkin)(3)(:)
	  if mini(ww)>0 then 
	    if nout>0 then
	      if sum(ww)==nout then
		bllst(blkin)(2)(portin)=nout
	      else
		bad_connection(corinv(blkin))
		ok=%f;return
	      end
	    else
	      bllst(blkin)(2)(portin)=sum(ww)
	      ok=%f
	    end
	  else      
	    nww=ww(find(ww<0))
	    if norm(nww-nww(1),1)==0 & nout>0 then
	      bllst(blkin)(2)(portin)=nout
	      k=(nout-sum(ww(find(ww>0))))/size(nww,'*')
	      if k==int(k) then
		bllst(blkin)(3)(find(ww<0))=k
	      else
		bad_connection(corinv(blkin))
		ok=%f;return
	      end
	    else
	      ok=%f
	    end
	  end
	elseif (nout==0) then
	  ww=bllst(blkout)(2)(:)
	  if mini(ww)>0 then 
	    if nin>0 then
	      if sum(ww)==nin then
		bllst(blkout)(3)(portout)=nin
	      else
		bad_connection(corinv(blkout))
		ok=%f;return
	      end
	    else
	      bllst(blkout)(3)(portout)=sum(ww)
	      ok=%f
	    end
	  else      
	    nww=ww(find(ww<0))
	    if norm(nww-nww(1),1)==0 & nin>0 then
	      bllst(blkout)(3)(portout)=nin
	      k=(nout-sum(ww(find(ww>0))))/size(nww,'*')
	      if k==int(k) then
		bllst(blkout)(2)(find(ww<0))=k
	      else
		bad_connection(corinv(blkout))
		ok=%f;return
	      end
	    else
	      ok=%f
	    end
	  end	
	else
	  //case where both are negative
	  ok=%f
	end
      end
    end
  end
  if ok then return, end
end
message('Not enough information to determine port sizes');  
ok=%f

function [outoin,outoinptr]=connmat(inpptr,outptr,inplnk,outlnk)
outoin=[];outoinptr=1
for k=1:outptr($)-1
  ii=[]
  for j=outlnk(k)
    ii=[ii;find(inplnk==j)]
  end
  outoini=[];jj=0
  for j=ii
    m=maxi(find(inpptr<=j))
    n=j-inpptr(m)+1
    outoini=[outoini;[m,n]]
    jj=jj+1
  end
  outoinptr=[outoinptr;outoinptr($)+jj]
  outoin=[outoin;outoini]
end
