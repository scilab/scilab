function [%pt,scs_m]=prt_align(%pt,scs_m)
// Copyright INRIA
while %t
  if %pt==[] then
    [btn,xc1,yc1,win,Cmenu]=cosclick()
    if Cmenu<>[] then
      %pt=[]
      [Cmenu]=resume(Cmenu)
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc1;yc1];
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
      [%win,Cmenu]=resume(win,Cmenu)
    end
  else
    xc1=%pt(1);yc1=%pt(2);win=%win;%pt=[]
  end  
  k1=getblock(scs_m,[xc1;yc1])
  if k1<>[] then o1=scs_m(k1);break,end
end
//
while %t
  [btn,xc2,yc2]=xclick(0)
  k2=getblock(scs_m,[xc2;yc2])
  if k2<>[] then o2=scs_m(k2);break,end
end
if get_connected(scs_m,k2)<>[] then
  message('Connected block can''t be aligned')
  return
end
//


[xout,yout,typout]=getoutputs(o1)
[xin,yin,typin]=getinputs(o1)
xx1=[xout xin]
yy1=[yout,yin]


//
[xout,yout,typout]=getoutputs(o2)
[xin,yin,typin]=getinputs(o2)
xx2=[xout xin]
yy2=[yout,yin]
if xx2==[]|yy2==[] then //one block has no port
  graphics2=o2(2);orig2=graphics2(1)
  graphics1=o1(2);orig1=graphics1(1)
  if abs(xc1-xc2)<abs(yc1-yc2) then //align vertically
    orig2(1)=orig1(1)
  else
    orig2(2)=orig1(2)
  end
else


  [m,kp1]=mini((yc1-yy1)^2+(xc1-xx1)^2)
  [m,kp2]=mini((yc2-yy2)^2+(xc2-xx2)^2)
  //
  xx1=xx1(kp1);yy1=yy1(kp1)
  xx2=xx2(kp2);yy2=yy2(kp2)


  graphics2=o2(2);orig2=graphics2(1)
  if abs(xx1-xx2)<abs(yy1-yy2) then //align vertically
    orig2(1)=orig2(1)-xx2+xx1
  else //align horizontally
    orig2(2)=orig2(2)-yy2+yy1
  end
end
graphics2(1)=orig2
drawobj(o2) // rubbout block
o2(2)=graphics2
drawobj(o2)
scs_m_save=scs_m
scs_m(k2)=o2
[scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)


