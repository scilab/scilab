function prt=splitted_links(scs_m,keep,del)
// given a vectors of indexes of "kept" blocks and "deleted" blocks 
// forms at table prt. Each line of prt is relative to a link beetween 
// a kept and a deleted block as follow:
// [io,keep_block_# port_# link_type link_color del_block_# port_#]
// io = 1 : link goes to a kept block, 0:link comes from a kept block
// Copyright INRIA
prt=[]
for kkeep=1:size(keep,'*')
  kk=keep(kkeep)
  o=scs_m(kk)
  if o(1)=='Block' then
    connected=get_connected(scs_m,kk,'in')
    for kc=1:size(connected,'*') //loop on input links 
      lk=scs_m(connected(kc))
      ct=lk(7)
      from=lk(8)
      to=lk(9)
      if or(from(1)==del) then // link between keep and del
	prt=[prt;[1,to(1),to(2),ct(2),ct(1),from(1),from(2)]]
      end
    end
    connected=get_connected(scs_m,kk,'clkin')
    for kc=1:size(connected,'*') //loop on input links 
      lk=scs_m(connected(kc))
      ct=lk(7)
      from=lk(8)
      to=lk(9)
      if or(from(1)==del) then // link between keep and del
	prt=[prt;[1,to(1),to(2),ct(2),ct(1),from(1),from(2)]]
      end
    end    
    
    connected=get_connected(scs_m,kk,'out')
    for kc=1:size(connected,'*') //loop on output links 
      lk=scs_m(connected(kc))
      ct=lk(7)
      from=lk(8)
      to=lk(9)
      if or(to(1)==del) then // link between keep and del
	prt=[prt;[0,from(1),from(2),ct(2),ct(1),to(1),to(2)]]
      end
    end    
    connected=get_connected(scs_m,kk,'clkout')
    for kc=1:size(connected,'*') //loop on output links 
      lk=scs_m(connected(kc))
      ct=lk(7)
      from=lk(8)
      to=lk(9)
      if or(to(1)==del) then // link between keep and del
	prt=[prt;[0,from(1),from(2),ct(2),ct(1),to(1),to(2)]]
      end
    end        
  end
end    

