function lst=mmodlst(lst)
// mmodlst is used to reduce mutiple concatenations, obtained by the 
// interpretor, such as 
// [[a,b],c]
// [[a;b];c]
// to a single one  whenever possible 
//!
// Copyright INRIA
void=['0','0','0','0']
nlst=size(lst);top=0
ilst=0
pos=[]
to_kill=[]
while ilst<nlst
  ilst=ilst+1
  if type(lst(ilst))==15 then
    lst(ilst)=mmodlst(lst(ilst))
  else
    op=lst(ilst)
    if type(op)<>10 then op='????',end  //bug dans macr2lst
    opn=op(1)
    if opn=='5' then
      if op(2)=='23' then // row concatenation
        i2=pos(top);i1=pos(top-1)
        a1=lst(i1)
        a2=lst(i2);
        // [a1 a2] contenation 
        if a1(1:2)==['5','23'] then 
          // [a1,a2] is [[a,b,...],a2] replaced by [a,b,...,a2 ]
          lst(i1)=void;to_kill=[to_kill,i1] //ignore concat which forms a1
          lst(ilst)(3)=addf(a1(3),'1'); //change rhs of current concat
          top=top-1
          pos(top)=ilst
        else // catenate
          top=top-1
          pos(top)=ilst
        end
      elseif op(2)=='27' then // column  concatenation
        i2=pos(top);i1=pos(top-1)
        a1=lst(i1)
        if size(a1,2)<4 then a1(4)=' ',end

        a2=lst(i2)
	if size(a2,2)<4 then a2(4)=' ',end

        // [a1;a2] contenation 
        if a1(1:2)==['5','27'] then
          // [a1;a2] is [[a;b;...];a2] replaced by [a;b;...;a2 ]
          lst(i1)=void;to_kill=[to_kill,i1]//ignore concat which forms a1
          lst(ilst)(3)=addf(a1(3),'1');//change rhs of current concat
          top=top-1
          pos(top)=ilst
        elseif and(a1(1:2)==['5','23']&a2(1:2)==['5','23'])&a1(3)==a2(3) then
          // [a1;a2] is [[a,b,...];[x,y,..] replaced by [a,b,...;x,y,..]
          lst(i1)=void;to_kill=[to_kill,i1]//ignore concat which forms a1
          lst(i2)=void;to_kill=[to_kill,i2]//ignore concat which forms a2
          lst(ilst)=['5','33','2',a1(3)];// change op
	  top=top-1
          pos(top)=ilst
        elseif and(a1(1:2)==['5','33']&a2(1:2)==['5','23'])&a1(4)==a2(3) then
          // [a1;a2] is [[[a,b,...;x,y,..];a2] replaced by [a,b,...;x,y,..;a2]
	  w=lst(i1)
	  lst(i1)=void;to_kill=[to_kill,i1]//ignore concat which forms a1
          lst(i2)=void;to_kill=[to_kill,i2]//ignore concat which forms a2
	  lst(ilst)=w
          lst(ilst)(3)=addf(a1(3),'1');//change rhs of current concat 
          top=top-1
          pos(top)=ilst
        else // catenate
          top=top-1
          pos(top)=ilst
	end

      else
        rhs=abs(evstr(op(3)));lhs=evstr(op(4))
        pos((top-rhs+1):(top-rhs+lhs))=ones(lhs,1)*ilst
        top=top-rhs+lhs
        pos(top+1:$)=[]
      end

    elseif opn=='20' then
      rhs=abs(evstr(op(3)));lhs=evstr(op(4))
      pos((top-rhs+1):(top-rhs+lhs))=ones(lhs,1)*ilst
      top=top-rhs+lhs
      pos(top+1:$)=[]
    elseif opn=='2'|opn=='3'|opn=='4'|opn=='6'|opn=='23' then
      top=top+1
      pos(top)=ilst
      //    else
    end
  end
end
// purge list of suppressed concatenations
to_kill=sort(to_kill)
for k=1:prod(size(to_kill))
  lst(to_kill(k))=null();
end



