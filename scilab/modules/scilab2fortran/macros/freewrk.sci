function nwrk=freewrk(nwrk,name)
//cette macro libere la place occupee par la variable dont le nom est
//donne dans name
//!
//  write(6,'-----------------'+name);
// Copyright INRIA
if part(name,1:7)=='work(iw' then
     ext=part(name,8:length(name)-1)
     if isnum(ext) then
       nb=evstr(ext)+1
       nw2=nwrk(2);
       nw2(2,nb)='0'
//     write(6,'libere :'+nw2(1,nb))
       nwrk(2)=nw2
     end
  elseif part(name,1:9)=='iwork(iiw' then
     ext=part(name,10:length(name)-1)
     if isnum(ext) then
       nb=evstr(ext)+1
       nw5=nwrk(5)
       nw5(2,nb)='0'
       nwrk(5)=nw5
     end
  end
endfunction
