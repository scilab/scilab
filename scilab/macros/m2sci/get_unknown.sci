function [vnms,vtps,stk,txt]=get_unknown(nam)
// handle case where nam appear in an expression while it is not known.

// nam is unknown here, it may be:
// - variable created in an other clause part
// - an m-file called without args 
// - a variable created by an eval

vnms;vtps;stk;txt;   

// check clause
if level(1)>1 then // in a clause
  k_n=find(vnms_n(:,2)==nam)
  if k_n<>[] // variable has been defined in an other clause part
    stk(top)=list(nam,'0',vtps_n(k_n)(2),vtps_n(k_n)(3),vtps_n(k_n)(1))
    vnms=[vnms;[nam,nam]]
    vtps($+1)=vtps_n(k_n)
    return
  end
end

// check for following assignments [x,..]=nam
lhs=0
il=ilst+1
nlst=size(lst)
while il<=nlst
  if lst(il)(1)<>'1' then break,end
  lhs=lhs+1
  il=il+1
end


// check if instruction is simply "nam" (nam=nam)
if lhs<>0 then
  if lhs==1 then
    if lst(ilst+1)(2)==nam then 
      // instruction is simply "nam". suppose it is an m-file call
      stk(top)=list()
      return
    end
  else
   // at least two lhs --> a m_file call
   stk(top)=list()
   return
  end
end
if or(nam==known_untranslated()) then
   stk(top)=list()
   return
end

//I am not able to determine what is nam
set_infos('mtlb('+nam+') can be replaced by '+nam+'() or '+nam+' whether '+nam+..
    ' is an m-file or not',1)

    
stk(top)=list('mtlb('+nam+')','0','?','?','?')
endfunction
