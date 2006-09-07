function count=get_profile(lst,ilst)
count=[]
ops=0
nlst=size(lst)
while ilst<=nlst then
  if type(lst(ilst))==15 then
    typ=lst(ilst)(1)
    c1=[]
    for ic=2:size(lst(ilst))
      c1=[c1;
	  get_profile(lst(ilst)(ic),1)]
    end
    count=[count;c1]
    ilst=ilst+1
  else
    ops=ops+1
    if lst(ilst)(1)=='25' then
      count=[count;[evstr(lst(ilst)(2:3)),max(0,ops-2)]];
      ops=0
    end
  end
  ilst=ilst+1
end
endfunction
