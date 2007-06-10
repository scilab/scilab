function count=get_profile(lst,ilst)
count=[]
ops=0
nlst=size(lst)
while ilst<=nlst then
  if type(lst(ilst))==15 then
    typ=lst(ilst)(1)
    c1=[]
    for ic=2:size(lst(ilst))
      c1=[c1;get_profile(lst(ilst)(ic),1)]
    end
    count=[count;c1]
    ilst=ilst+1
  else
    ops=ops+1
     //real timing line found
    if lst(ilst)(1)=='25' then  
      count=[count;[evstr(lst(ilst)(2:3)),max(0,ops-2)]];
      ops=0
    end
    //possibly, non interpreted code line (e.g. subfunction definition header)
    if lst(ilst)(1)=='3' then 
      ncode=1
    end
    //non interpreted code lines (e.g. subfunction definition body)
    if lst(ilst)(1)=='26' then
      // macr2lst keeps whitespace (even too much... leading whitespace
      //   makes a new code line), fun2string no. See bug 2413
      codelines=lst(ilst); codelines=codelines(4:$); 
      ncode=length(stripblanks(codelines)<>"")
    end
    //subfunction definition trailer
    if lst(ilst)(1)=='20' &  lst(ilst)(2)=="deff" then 
      count=[count;zeros(ncode,3)];      
    end
  end
  ilst=ilst+1
end
endfunction
