function [P,r]=augment(G,SRT,flag)
// Augmented plants P
// flag='output' (default) :
//
//       [ I | -G]   -->'S'
//       [ 0 |  I]   -->'R'
//   P = [ 0 |  G]   -->'T'
//       [-------]
//       [ I | -G]
//
//
// flag='input' :
//
//       [ I | -I]   -->'S'
//       [ G | -G]   -->'R'
//   P = [ 0 |  I]   -->'T'
//       [-------]
//       [ G | -G]      
//!
// Copyright INRIA
[LHS,RHS]=argn(0);
if RHS <= 2 then flag='output';end
select part(flag,1)
case 'o'
  G1=G(1);
  if RHS==1 then SRT='SRT';end
  r=size(G);
  [ny,nu]=size(G);Iu=eye(nu,nu);Iy=eye(ny,ny);
  Ouy=zeros(nu,ny);Oyu=zeros(ny,nu);Ouu=zeros(nu,nu);
  Oyy=zeros(ny,ny);
  ssSRT=0;
  if G1(1)=='r' then ssSRT=1;end
  long=length(SRT);
  select long
  case 3 then
    // 'SRT'
    if SRT<>"SRT" then error("option ''"+string(SRT)+"'' not allowed, use ''SRT'' ",9999);
      return
    end
    if ssSRT==0 then
      W1=[Iy,Oyu,Oyy;
	  Ouy,Iu,Ouy;
	  -Iy,Oyu,Iy;
	  Iy,Oyu,Oyy];
      W2=[Iy,-G;
	  Ouy,Iu;
	  Iy,Oyu];
      P=W1*W2;
    end
    if ssSRT ==1 then
      P=[Iy,-G;
	  Ouy,Iu;
	  Oyy,G;
	  Iy,-G]
    end
    return  
  case 2 then
    if SRT=='SR' then
      if ssSRT==0 then
	W1=[Iy,Oyu,Oyy;
	    Ouy,Iu,Ouy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end
      if ssSRT==1 then
	P=[Iy,-G;
	    Ouy,Iu;
	    Iy,-G];
      end
      return
    end
    if SRT=='ST' then
      if ssSRT==0 then
	W1=[Iy,Oyu,Oyy;
	    -Iy,Oyu,Iy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end
      if ssSRT ==1 then
	P=[Iy, -G;
	    Oyy, G;
	    Iy, -G];
      end
      return
    end
    if SRT=='RT' then
      if ssSRT==0 then
	W1=[Ouy,Iu,Ouy;
	    -Iy,Oyu,Iy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end;
      if ssSRT ==1 then
	P=[Ouy,Iu;
	    Oyy,G;
	    Iy,-G];
      end
      return  
    end 
    error("option ''"+string(SRT)+"'' not allowed use ''SR'', ''ST'' or ''RT'' ",9999);
  case 1 then
    if SRT=='S' then
      if ssSRT==0 then
	W1=[Iy,Oyu,Oyy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end;
      if ssSRT ==1 then
	P=[Iy,-G;
	    Iy,-G];
      end
      return
    end
    if SRT=='R' then
      if ssSRT==0 then
	W1=[Ouy,Iu,Ouy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end
      if ssSRT ==1 then
	P=[Ouy,Iu;
	    Iy,-G];
      end
      return
    end
    if SRT=='T' then
      if ssSRT==0 then
	W1=[-Iy,Oyu,Iy;
	    Iy,Oyu,Oyy];
	W2=[Iy,-G;
	    Ouy,Iu;
	    Iy,Oyu];
	P=W1*W2;
      end
      if ssSRT ==1 then
	P=[Oyy,G;
	    Iy,-G];
      end
      return
    end
  end
case 'i'
  G1=G(1);
  if RHS==1 then SRT='SRT';end
  r=size(G);
  [ny,nu]=size(G);Iu=eye(nu,nu);Iy=eye(ny,ny);
  Ouy=zeros(nu,ny);Oyu=zeros(ny,nu);Ouu=zeros(nu,nu);
  Oyy=zeros(ny,ny);
  ssSRT=0;
  if G1(1)=='r' then ssSRT=1;end
  long=length(SRT);
  select long
  case 3 then
    // 'SRT'
    if SRT<>"SRT" then error("option ''"+string(SRT)+"'' not allowed, use ''SRT'' ",9999);
      return;
    end;
    if ssSRT==0 then
      W1=[Iu,-Iu;
	  Oyu,Oyu;
	  Ouu,Iu;
	  Oyu,Oyu];
      W2=[Ouy;Iy;Ouy;Iy];
      W3=[Iu,-Iu];
      P=W1+W2*G*W3;
    end
    if ssSRT ==1 then
      P=[Iu,-Iu;
	  G,-G;
	  Ouu,Iu;
	  -G,G]
    end
    return  
  case 2 then
    if SRT=='SR' then
      if ssSRT==0 then
	W1=[Iu,-Iu;
	    Oyu,Oyu;
	    Oyu,Oyu];
	W2=[Ouy;Iy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;
      end
      if ssSRT==1 then
	P=[Iu,-Iu;
	    G,-G;
	    -G,G]   
      end
      return
    end
    if SRT=='ST' then
      if ssSRT==0 then
	W1=[Iu,-Iu;
	    Ouu,Iu;
	    Oyu,Oyu];
	W2=[Ouy;Ouy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;
      end
      if ssSRT ==1 then
	P=[Iu,-Iu;
	    Ouu,Iu;
	    -G,G]
      end
      return
    end
    if SRT=='RT' then
      if ssSRT==0 then
	W1=[Oyu,Oyu;
	    Ouu,Iu;
	    Oyu,Oyu];
	W2=[Iy;Ouy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;
      end;
      if ssSRT ==1 then
	P=[G,-G;
	    Ouu,Iu;
	    -G,G]
      end
      return  
    end 
    error("option ''"+string(SRT)+"'' not allowed use ''SR'', ''ST'' or ''RT'' ",9999);
  case 1 then
    if SRT=='S' then
      if ssSRT==0 then
	W1=[Iu,-Iu;
	    Oyu,Oyu];
	W2=[Ouy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;

      end;
      if ssSRT ==1 then
	P=[Iu,-Iu;
	    -G,G] 
      end
      return
    end
    if SRT=='R' then
      if ssSRT==0 then
	W1=[Oyu,Oyu;
	    Oyu,Oyu];
	W2=[Iy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;
      end
      if ssSRT ==1 then
	P=[G,-G;
	    -G,G]
      end
      return
    end
    if SRT=='T' then
      if ssSRT==0 then
	W1=[Ouu,Iu;
	    Oyu,Oyu];
	W2=[Ouy;Iy];
	W3=[Iu,-Iu];
	P=W1+W2*G*W3;
      end
      if ssSRT ==1 then
	P=[Ouu,Iu;
	    -G,G]
      end
      return
    end
  end
end
endfunction
