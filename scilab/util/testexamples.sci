function r=load_ref(name)
  if exists(name)==0 then r=%f;return,end
  v=evstr(name)
  if type(v) == 9 then   v = ghdl2tree(v);end,
  execstr(name+'_ref=v;load(%U,'''+name+'_ref'+''');r=%CMP(v,'+name+'_ref);')
endfunction
function r=%xdel(w,opt)
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  xdel_build one.
  r=%f
  if winsid()==[] then return,end
   cur=xget('window')
 //  
  if argn(2)==1 then
    ids_=[]
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
    end
    xdel(w)
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window')
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_,%wins_ref) then r=%t,return,end
    xdel()
  end
  if or(winsid()==cur) then xset('window',cur),end
endfunction
function r=%clf(w,opt)
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  clf_build one.

  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  rhs=argn(2)
  if rhs==1&type(w)==10 then opt=w;rhs=0,end
  if winsid()==[] then return,end
  //
  if rhs==1 then
    ids_=[]
    if type(w)==9 then 
      H=w;w=[]
      for h=H,w=[w,h.figure_id],end
    end
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
    end
    if rhs==1 then clf(w),else clf(w,opt),end
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window')
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_,%wins_ref) then r=%t,return,end
    clf()
  end
  if or(winsid()==cur) then xset('window',cur),end
endfunction

function r=%xbasc(w)
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  xbasc_build one.
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  //
  if argn(2)==1 then
    ids_=[]
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
      
    end
    xbasc(w)
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window');
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_, %wins_ref) then r=%t,return,end
    xbasc()
  end
  if or(winsid()==cur) then xset('window',cur),end
endfunction
function r=%CMP(A,B)
//Author : Serge Steer, april 2005, Copyright INRIA
//  
// thsi function compares two variables, floating points data are
// compared using a relative tolerance
  r=%f
  tol=0.00001
  if type(A)<>type(B) then r=%t,return,end
  select type(A)
  case 1 then //float
    if or(size(A)<>size(B)) then  r=%t,return,end
    ka=~isnan(A);kb=~isnan(B);
    if or(ka<>kb)  then  r=%t,return,end
    if norm(A(ka)-B(kb))/max(1,norm(A(ka)))>tol then  r=%t,return,end
  case 2 then //polynomial
    if or(size(A)<>size(B)) then  r=%t,return,end
    if or(degree(A)<>degree(B)) then r=%t,return,end  
    for k=1:size(A,'*')
      if %CMP(coeff(A(k)),coeff(B(k))) then r=%t,return,end
    end
  case 4 then //boolean
    if or(size(A)<>size(B)) then  r=%t,return,end
    if or(A<>B) then  r=%t,return,end
  case 5 then //sparse
    if or(size(A)<>size(B)) then  r=%t,return,end
    [ija,A]=spget(A);[ijb,B]=spget(B);
    if or(ija<>ijb) then  r=%t,return,end
    ka=~isnan(A);kb=~isnan(B);
    if or(ka<>kb)  then  r=%t,return,end
    if norm(A(ka)-B(ka))/max(1,norm(A(ka)))>tol then  r=%t,return,end
  case 6 then //boolean sparse
    if or(size(A)<>size(B)) then  r=%t,return,end
    if or(A<>B) then  r=%t,return,end
  case 8 then //int
    if or(size(A)<>size(B)) then  r=%t,return,end
    if or(A<>B) then  r=%t,return,end
  case 9 then //handle
    if or(size(A)<>size(B)) then  r=%t,return,end
//    if or(A<>B) then  r=%t,return,end
  case 10 then //string
    if or(size(A)<>size(B)) then  r=%t,return,end
    if or(A<>B) then  r=%t,return,end
  case 13 then //compiled function
     if A<>B then  r=%t,return,end
  case 14 then //library
     if A<>B then  r=%t,return,end
  case 15 then //list
     if or(lstsize(A)<>lstsize(B)) then  r=%t,return,end
     if or(definedfields(A)<>definedfields(B)) then r=%t,return,end
     for k = definedfields(A)
       if %CMP(A(k),B(k)) then r=%t,return,end
     end
  case 16 then //tlist
     if or(lstsize(A)<>lstsize(B)) then  r=%t,return,end
     if or(definedfields(A)<>definedfields(B)) then r=%t,return,end
     for k = definedfields(A)
       if %CMP(A(k),B(k)) then r=%t,return,end
     end
  case 17 then //mlist
     if or(lstsize(A)<>lstsize(B)) then  r=%t,return,end
     if or(definedfields(A)<>definedfields(B)) then r=%t,return,end
     for k = definedfields(A)
       if %CMP(getfield(k,A),getfield(k,B)) then r=%t,return,end
     end
  case 130 then
    if A<>B then  r=%t,return,end
  else
    r=%f
  end
endfunction
