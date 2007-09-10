function txt=fun2string(fun,nam)
// Copyright INRIA
// Author Serge Steer  
// form  syntax to scilab of a compiled function
//%Parameters
// lst   : list, represents the interpreted code of the Scilab function given by macr2lst
// nam   : nam of the scilab function to generate
// txt   : character string column vector: the text of resulting scilab function
//!
  if argn(2)<2 then nam='ans',end
  if type(fun)==11 then comp(fun),end
  if type(fun)<>15 then
    lst=macr2lst(fun);
  else
    lst=fun
  end
  lst=mmodlst(lst)

  lcount=1;level=[0,0];
  quote=''''
  dquote='""'
  CR='\'+ascii(10)
  opstable()
  // add input variable in the defined variables
  inputs=lst(3);outputs=lst(2)
  sciexp=0
//  cmt=lst(4)(1)=='31'
  crp=ins2sci(lst,4)
  //add the function header
  select size(outputs,'*')
  case 0 then outputs='[]'
  case 1 then outputs=outputs
  else outputs=lhsargs(outputs)
  end
  select size(inputs,'*')
  case 0 then inputs=''
  else inputs=rhsargs(inputs)
  end

  hdr='function '+outputs+'='+nam+inputs;
  crp1=crp(1)
  if crp1<>'' then crp1=','+crp1,end
  if crp(2)=='' then
    txt=[hdr+crp1;'  '+crp(3:$-2);'endfunction']
  else
    txt=[hdr+crp1;'  '+crp(2:$-2);'endfunction']
  end
endfunction

function txt=ins2sci(lst,ilst)
// traduit un ensemble d'instructions debutant a l'adresse ilst de la
// liste courante lst
//!
// Copyright INRIA
// Author Serge Steer  
  nlst=size(lst)
  level;level(2)=0;
  txt=''
  while ilst<=nlst then
    if type(lst(ilst))==15 then
      t1=cla2sci(lst(ilst))
      ilst=ilst+1
    else
      if lst(ilst)(1)=='15' then ilst=ilst+1;txt($+1)='';end
      [t1,ilst]=cod2sci(lst,ilst)
      end
    txt=catcode(txt,t1)
  end
endfunction
function txt=cla2sci(clause)
// traduit une clause (if while for select)
//!
// Copyright INRIA
// Author Serge Steer  
  typ=clause(1)
  txt=''
  //write(6,'cla2sci '+typ(1))
  level;level(1)=level(1)+1
  select typ(1)
  case 'if' then
    ncl=size(clause)
    ncas=(ncl-2)/2
    tg=''
    level(2)=1
    [t1,t2,ilst]=exp2sci(clause(2),1)
    t1=t1(1);
    tg=catcode(tg,t2)
    txt=catcode('if ',catcode(splitexp(t1(1)),' then '))
    t1=ins2sci(clause(3),1)
    txt=catcode(txt,indentsci(t1));
    // elseif parts
    for ic=2:ncas
      level(2)=ic
      [t1,t2,ilst]=exp2sci(clause(2*ic),1)
      if type(t1(1))==15 then t1=t1(1),end
      tg=catcode(tg,t2)
      txt=catcode(txt,catcode('elseif ',catcode(splitexp(t1(1)),' then ')));
      t1=ins2sci(clause(1+2*ic),1)
      txt=catcode(txt,indentsci(t1));
    end;
    // else part
    t1=ins2sci(clause(ncl),1)
    if or(t1<>'') then txt=catcode(txt,catcode('else ',indentsci(t1)));end
    txt=catcode(tg,catcode(txt,'end,'))
  case 'while' then 
    level(2)=1
    [t1,t2,ilst]=exp2sci(clause(2),1)
    t1=t1(1);
    txt=catcode('while ',catcode(splitexp(t1(1)),' then'))
    t1=ins2sci(clause(3),1)
    txt=catcode(txt,catcode(indentsci(t1),'end,'))
  case 'for' then
    name=typ(2)
    sciexp=1
    level(2)=1
    [t1,t2,ilst]=exp2sci(clause(2),1)
    t1=t1(1)
    txt=catcode('for '+name+' = ',splitexp(t1(1)+','));
    sciexp=0;
    t1=ins2sci(clause(3),1)
    txt=catcode(txt,catcode(indentsci(t1),'end,'))
  case 'select' then
    ncas=(size(clause)-3)/2
    tg=''
    level(2)=1
    [exp1,t1,ilst]=exp2sci(clause(2),1)
    exp1=exp1(1)
    txt=catcode('select ',splitexp(exp1(1)))
    
    for ic=2:2:2*ncas
      level(2)=ic/2
      [exp2,t2,ilst]=exp2sci(clause(1+ic),1)
      tg=catcode(tg,t2)
      exp2=exp2(1)
      txt=catcode(txt,catcode('case ',catcode(splitexp(exp2(1)),' then')))
      t1=ins2sci(clause(2+ic),1)
      txt=catcode(txt,indentsci(t1));
    end;
    t1=ins2sci(clause(3+2*ncas),1)
    if or(t1<>'') then
      txt=catcode(txt,catcode('else',indentsci(t1)));
    end
    txt=catcode(tg,catcode(txt,'end,'))
  case 'try' then
    if size(clause(2))==0 then
      t1=catcode('try',';')
    else
      t1=catcode('try',indentsci(ins2sci(clause(2),1)))
    end
    if size(clause(3))==0 then
      t2='end,'
    else
      t2=catcode(catcode('catch',indentsci(ins2sci(clause(3),1))),'end,')
    end
    txt=catcode(txt,catcode(t1,t2))
  end
endfunction

function [txt,ilst]=cod2sci(lst,ilst)
//
//!
// Copyright INRIA
// Author Serge Steer 
  nlst=size(lst)
  txt=''
  ilst=ilst-1
  while ilst<nlst then
    ilst=ilst+1
    op=lst(ilst)
    if type(op)==15 then return,end
    select op(1)
    case '1' then //stackp (retained for compatibility with 2.7 and earlier version)
      prev=lst(ilst-1)
      if size(prev,'*')==1 then prev=[prev ' '],end
      if and(prev(1:2)==['5','25'])|prev(1)=='20' then
	lhs=evstr(prev(4))
      else
	lhs=1;rhs=1
      end
      if lhs==1 then
	expk=stk(1);
	opk=lst(ilst);ilst=ilst+1
	opk2=opk(2)
	if expk(2)<>'-1'& expk(2)<>'-2' then
	  if opk(2)=='ans' then
	    expk(1)($)= expk(1)($)+';'
	    txt=catcode(txt,splitexp(expk(1)(1)))
	  else
	    w=expk(1)(1)
	    w=opk2+' = '+w(1)+';'
	    w=splitexp(w)
	    txt=catcode(txt,w)
	  end
	end
      else //
	LHS=[]
	for k=1:lhs
	  opk=lst(ilst);ilst=ilst+1;
	  opk2=opk(2);
	  LHS=[opk2,LHS];
	end
	if stk(1)(2)=='-1' then // variable has  not been previously stored
	  txt=catcode(txt,lhsargs(LHS)+' = '+stk(1)(1)+';')
	else
	  RHS=[]
	  for k=1:min(lhs,size(stk))
	    expk=stk(k);
	    RHS=[RHS,expk(1)];
	  end
	  txt=catcode(txt,lhsargs(LHS)+' = '+rhsargs(RHS)+';')
	end
      end
      if ilst<=nlst then
	if lst(ilst)(1)<>'15' then 
	  ilst=ilst-1,
	else
	  txt($+1)=''
	  lcount=lcount+1
	end
      end
    case '12' then //pause
      txt=catcode(txt,'pause,')
    case '13' then //break
      txt=catcode(txt,'break,')
    case '14' then //abort
      txt=catcode(txt,'abort,')
    case '15' then ,//eol
      txt($+1)=''
    case '18' then  
    case '25' then   
    case '28' then //continue
      txt=catcode(txt,'continue,')
    case '29' then //affectation
      ip=evstr(op(2))
      if ip==99|ip==2 then 
	ip=''
      else
	ip=code2str(ip)
      end
      op=matrix(op(3:$),2,-1)
      lhs=size(op,2)
      LHS=[]
      iind=0
      for k=1:lhs
	name=op(1,k)
	rhs=evstr(op(2,k))
	if rhs==0 then
	  LHS=[name,LHS]
	else
	  if  rhs==1 then //x(i)=... or x.y.z(i,j..)=..
	    iind=iind+1,
	    p=''
	    P=stk(iind) //Path
	    if type(P(1))==1 then //x.y.z(i,j..)=..
	      for ii=2:size(P)
		Pi=P(ii)
		if type(Pi(1))==1 then // ...(i,j,..)
		  Ii=[];for ind=2:size(Pi),Ii=[Ii,Pi(ind)(1)],end
		  p=p+'('+strcat(Ii ,', ')+')'
		else
		  if and(strindex(Pi(1),'''')==[1 length(Pi(1))]) then
		    p=p+'.'+evstr(Pi(1))
		  else
		    p=p+'('+Pi(1)+')'
		  end
		end
	      end
	    else // /x(i)=...
	      p='('+P(1)+')'
	    end
	    LHS=[name+p,LHS]
	  else //rhs>1 x(i,j,..)=...
	    I=[];
	    for i=1:rhs, iind=iind+1,I=[I,string(stk(iind)(1))];end
	    LHS=[name+'('+strcat(I,', ')+')',LHS]
 	  end
	end
      end
      if lhs>1 then  LHS='['+strcat(LHS,',')+']',end
      RHS=[]
      for k=iind+1:size(stk), RHS=[RHS stk(k)(1)],end
      if size(RHS,'*')<>1, RHS='('+strcat(RHS,',')+')',end
      if LHS=='ans' then
	txt=catcode(txt,RHS+ip)
      else
	if size(LHS,'*')==1&and(LHS==RHS) then
	  txt=catcode(txt,RHS+ip)
	else
	  txt=catcode(txt,LHS+' = '+RHS+ip)
	end
      end
      txt=splitexp(txt)
    case '31' then //comment
      txt=catcode(txt,'//'+op(2))
    case '99' then //return
      txt=catcode(txt,'return,')
    else
      [stk,t1,ilst]=exp2sci(lst,ilst);
      if size(t1,1)==1 then t1=splitexp(t1),end
      txt=catcode(txt,t1);t1=''
      ilst=ilst-1
    end
  end
  ilst=ilst+1
endfunction

function [stk,txt,ilst]=exp2sci(lst,ilst)
// Copyright INRIA
// Author Serge Steer  
  nlst=size(lst)
  top=0
  stk=list()
  txt=''
  ilst=ilst-1
  cmod=0;
  //
  ok=%t
  while ilst<nlst&ok then
    lhs=1
    ilst=ilst+1
    op=lst(ilst)
    if type(op)==10 then
      if prod(size(op))==1 then op=[op ' '],end
      select op(1)
      case '0' then
	
      case '2' then //stackg
	if (op(3)=='-3'&op(4)<>'0') then
	  [stk,top]=get2sci(op(2),stk,top)
	elseif (op(3)=='-2'&op(4)<>'0') then
	  m=%t
	  if top>0 then
	    if type(stk(top)(1))==1 then //previous argument is a recursive index
	      top=top+1
	      stk(top)=list(op(2),'0')
	      m=%f
	    end
	  end
	  if m then
	    //appel des macros
	    op(3)=op(4)
	    op1=lst(ilst+1)
	    if op1(1)=='5'&op1(2)=='25' then
	      ilst=ilst+1
	      op(4)=op1(4)
	    else
	      op(4)='1'
	    end
	    [stk,top]=func2sci(op,stk)
	  end
	elseif op(3)=='-2'&op(4)=='0' then //instruction reduced to a name
	  [stk,top]=get2sci(op(2),stk,top)			   
	  ilst=ilst+1
	  lst;lst(ilst+1)(3)=op(2)
	elseif op(4)=='0' then
	  [stk,top]=get2sci(op(2),stk,top)
	else
	  if top>0 then
	    if type(stk(top)(1))==1 then
	      top=top+1
	      stk(top)=list(op(2),'0')
	    end
	  end
	end
	t1=[]
      case '3' then //string
	quote=''''
	dqote='""'
	top=top+1
	st=strsubst(strsubst(op(2),quote,quote+quote),dquote,dquote+dquote)
	stk(top)=list(quote+st+quote,'0')
      case '4' then //matrice vide
	top=top+1
	stk(top)=list('[]','0')
      case '5' then //allops
	t1=[]
	iop=evstr(op(2))
	top1=top
	execstr('[stkr,t1,top]=_'+ops(iop,2)+'2sci()')
	stk(top)=stkr
	txt=catcode(txt,t1)
	t1=''
      case '6' then //num
	[stk,top]=num2sci(op(2),stk)
      case '20' then //functions
	[stk,top]=func2sci(op,stk)
      case '15' then 
	if top>0 then
	  stk(top)(1)=stk(top)(1)+CR    
	  if size(txt,'*')==1 then stk(top)(1)=stk(top)(1)+txt;txt='';end
	else
	  txt($+1)=''
	end
      case '18' then  //named variable
	stk(top)(1)=op(2)+'='+ stk(top)(1)
      case '19' then  // mkindx
		      // replace all variables describing path by a single list
		      n=evstr(op(2))
		      m=evstr(op(3));
		      if m>1&n>0 then
			l=list(m),pos=top-m
			for k=1:m,l($+1)=stk(pos+k),end
			top=pos+1
			stk(top)=l
			//for k=2:m,stk(top+1)=null(),end
		      end
		      nn=n;if n==0 then nn=m,end
		      l=list(nn),pos=top-nn
		      for k=1:nn,l($+1)=stk(pos+k),end
		      top=pos+1
		      stk(top)=l
		      //for k=size(stk):-1:top+1,stk(k)=null(),end
      case '23' then   
	top=top+1
	stk(top)=list(quote+op(2)+quote,'0')
      case '24' then   
	top=top+1
	stk(top)=list('','0')  //list('null()','0')
      case '25' then  
      case '26' then 
	//      vector of string
	quote=''''
	dqote='""'
	if lst(ilst+1)(1)=='20'&lst(ilst+1)(2)=='deff' then
	  st=op(4:$)
	  fn='function '+strsubst(strsubst(stk(top)(1),quote,''),dquote,'')
	  st(stripblanks(st)=='')=[]
	  fn=[fn;st(:)],
	  fn=[fn;'endfunction']
	  txt=catcode(txt,fn)
	  lst(ilst+1)(1)='0'; ilst=ilst+1;// ignore deff (20)
	  stk(top)=list('','-2'),
	else
	  m=evstr(op(2))
	  n=evstr(op(3))
	  top=top+1
	  st=strsubst(strsubst(op(4:$),quote,quote+quote),dquote,dquote+dquote)
	  st=quote+st+quote;
	  if m*n>1 then
	    st(1)='['+st(1); st($)=st($)+']'
	    if m>1 then
	      tt=[];
	      for l=1:m
		tt=[tt;strcat(st(l:m:$),',')]
	      end
	      st=strcat(tt,';');
	    end
	  end
	  stk(top)=list(st,'0')
	end
      case '27' then
	// funptr variable
	top=top+1
	stk(top)=list(op(4),'0')
      case '31' then //comment into multi line matrix definition a=[...
	if top==0 then 
	  txt=[txt;'//'+op(2)]
	else
	  stk(top)(1)=stk(top)(1)+'; //'+op(2)
	end
      else
	ok=%f
      end
    else
      ok=%f
    end

  end
  stk=list(stk(1:top))
  lst=resume(lst)
endfunction

function [stk,top]=func2sci(op,stk)
// translate all functions calls
// Copyright INRIA
// Author Serge Steer 
  lhs=evstr(op(4)) 
  rhs=max(evstr(op(3)) ,0)
  [stkr,top1]=sci_gener(op(2))

  // add lhs expression to the stack
  top=top-rhs
  //  for k=size(stk):-1:(top-rhs+1), stk(top)=null(),end
//   if lhs>1 then
//     for k=1:lhs
//       top=top+1
//       stk(top)=stkr(k)
//     end
//   else
    top=top+1
    stk(top)=stkr
//  end
endfunction

function [stk,txt,top]=_m2sci()
  txt=[]
  s1=stk(top-1)
  s2=stk(top)

  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end
  stk=list(e1+' * '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_a2sci()
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);
  if s2(2)=='3' then s2(1)='('+s2(1)+')',end
  if s1(2)=='3' then s1(1)='('+s1(1)+')',end
  stk=list(s1(1)+' + '+s2(1),'2')
  top=top-1
endfunction

function [stk,top,txt]=get2sci(nam,stk,top)
// Translate the named variable acquisition
// Copyright INRIA  
// Author Serge Steer  
  txt=[]
  top=top+1
  stk(top)=list(nam,'0')
endfunction

function [stk,top]=sci_gener(nam)
// Copyright INRIA  
// Author Serge Steer  
  RHS=[]
  for k=1:rhs
    arg=stk(top)(1);
    if arg=='eye()' then arg=':',end
    RHS=[arg,RHS]
    top=top-1
  end
  if RHS==[] then RHS='',end
  top=top+1
//  if lhs==1 then
    stk=list(nam+rhsargs(RHS),'0')
//  else
//    stk=list()
//    for k=1:lhs
//      stk(k)=list(nam+rhsargs(RHS),'-1')
//    end
//  end
endfunction

function [stk,txt,top]=_c2sci()
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  m=evstr(op(3));n=evstr(op(4))
  top1=top-m*n
  if op(2)=='23' then  // row 
    nrc=m
    row=[]
    for k=1:nrc
      row=[row,stk(top1+k)(1)]
    end
    t='['+strcat(row,',')+']'
    stk=list('['+strcat(row,',')+']','0')
  elseif op(2)=='27' then // column
    ncc=m
    col=[]
    for l=1:ncc
      col=[col,stk(top1+l)(1)]
    end
    multi=%f
    //!multi-lines expressions not handed by other functions than stackp
    //if sum(length(col))>70 then 
    //  ii=ilst+1
    //  while lst(ii)(1)=='15' then ii=ii+1,end
    //  if lst(ii)(1)=='1' then multi=%t,end
    //end
    if multi then
      col(1)='['+col(1)
      col($)=col($)+']'
      stk=list(col,'0')
    else
      stk=list('['+strcat(col,';')+']','0')
    end
  else
    nrc=m;ncc=n
    col=[]
    for l=1:nrc
      row=[]
      for k=1:ncc
	row=[row,stk(top1+(l-1)*ncc+k)(1)];
      end
      col=[col,strcat(row,',')]
    end
    multi=%f
    //!multi-lines expressions not handed by other functions than stackp
    if sum(length(col))>70 then 
      ii=ilst+1
      while lst(ii)(1)=='15' then ii=ii+1,end
      if lst(ii)(1)=='1' then multi=%t,end
    end
    if multi then
      col(1)='['+col(1)
      col($)=col($)+']'
      stk=list(col,'0')
    else
      stk=list('['+strcat(col,';')+']','0')
    end
  end
  top=top1+1
endfunction

function opstable()
// Copyright INRIA
// Author Serge Steer  
  quote=''''
  logics=['==','<','>','<=','>=','<>']
  ops     =['+',   'a';
	    '-',    's';
	    '*',    'm';
	    '.*',   'x';
	    '.*.',  'k';
	    '*.',   'u';
	    '/',    'r';
	    './',   'd';
	    './.',  'y';
	    '/.',   'v';
	    '\',    'l';
	    '.\',   'q';
	    '.\.',  'z';
	    '\.',   'w';
	    '^',   'p';
	    '==', 'log';
	    '<', 'log';
	    '>', 'log';
	    '<=', 'log';
	    '>=', 'log';
	    '~=', 'log';
	    ':',    'imp';
	    '[]',   'c';
	    'ins',  'i';
	    'ext',  'e';
	    quote,  't';
	    '[]',   'c';
	    '|',    'g';
	    '&',    'h';
	    '~',    '5';
	    '.^',   'j';
	    '.'+quote '0';
	    '[]',   'c']         
  
  [logics,ops]=resume(logics,ops)
endfunction

function [stk,top]=num2sci(val,stk)
// traduit la definition d'un nombre
// Copyright INRIA
// Author Serge Steer  
  top=top+1
  stk(top)=list(val,'0')
endfunction

function [stk,txt,top]=_log2sci()
  txt=[]
  iop=evstr(op(2))
  s2=stk(top);s1=stk(top-1);top=top-1
  if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
  if s1(2)=='2'|s1(2)=='3' then s1(1)='('+s1(1)+')',end
  stk=list(s1(1)+' '+ops(iop,1)+' '+s2(1),'3')
endfunction

function txt=rhsargs(args)
// Copyright INRIA
// Author Serge Steer  
  txt='('+strcat(args,', ')+')'
endfunction

function txt=lhsargs(args)
// Copyright INRIA
// Author Serge Steer  
  txt='['+strcat(args,', ')+']'
endfunction

function [stk,txt,top]=_p2sci()
// ^
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);
  [s1,te1]=s1(1:2);
  [s2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then s2='('+s2+')',end
  if te1=='2'|te1=='3' then s1='('+s1+')',end
  if part(s2,1)=='-' then s2='('+s2+')',end
  stk=list(s1+'^'+s2,'2')
  top=top-1
endfunction

function [txt]=indentsci(txt)
// Copyright INRIA
// Author Serge Steer  
  k=find(txt<>'')
  if k<>[] then
    bl='  '
    txt(k)=bl(ones(prod(size(k)),1))+txt(k)
  end
endfunction

function lst=mmodlst(lst)
// mmodlst is used to reduce mutiple concatenations, obtained by the 
// interpretor, such as 
// [[a,b],c]
// [[a;b];c]
// to a single one  whenever possible 
// Copyright INRIA
// Author Serge Steer  
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
	if size(op,'*')<4 then pause,end
	rhs=max(evstr(op(3)),0);lhs=evstr(op(4))
	pos((top-rhs+1):(top-rhs+lhs))=ones(lhs,1)*ilst
	top=top-rhs+lhs
	pos(top+1:$)=[]
      elseif opn=='2'|opn=='3'|opn=='4'|opn=='6'|opn=='23'|opn=='24'|opn=='26'|opn=='27'|opn=='29' then
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
endfunction

function [stk,txt,top]=_02sci()
// translate .'
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top)
  if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
  stk=list(s2(1)+'.'+quote,'3')
endfunction

function [stk,txt,top]=_52sci()
// generate code for ~
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top)
  if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
  stk=list('~'+s2(1),'3')
endfunction

function [stk,txt,top]=_d2sci()
// ./
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);top=top-1;

  if  s2(2)=='3'|s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
  if s1(2)=='3'|s1(2)=='2' then s1(1)='('+s1(1)+')',end

  stk=list(s1(1)+' ./ '+s2(1),'1')
endfunction

function [stk,txt,top]=_e2sci()
// generate code for extraction
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  rhs=maxi(0,abs(evstr(op(3)))-1)
  sn=stk(top);top=top-1
  s2=stk(top)
  if rhs==1 then
    if type(s2(1))==1 then // recursive extraction
      n=s2(1)
      ex=sn(1)
      for k=1:n
	ik=s2(k+1)
	if type(ik(1))==1 then
	  ex1=[]
	  for k1=1:ik(1),ex1=[ex1,ik(1+k1)(1)],end
	  ex=ex+rhsargs(ex1)
	else
	  if ik(2)=='0'&part(ik(1),1)=='''' then
	    ex=ex+'.'+part(ik(1),2:length(ik(1))-1)
	  else
	    if ik(1)=='eye()' then ik(1)=':',end 
	    ex=ex+'('+ik(1)+')'
	  end
	end
      end
      stk=list(ex,'0')
    else
      if s2(1)=='eye()' then s2(1)=':',end
      stk=list(sn(1)+'('+s2(1)+')','0')
    end
  elseif rhs==0 then
    sn(1)=sn(1)+'()';
    stk=sn;
    top=top+1
  else
    s1=stk(top-1);top=top-1
    if s2(1)=='eye()' then s2(1)=':',end
    if s1(1)=='eye()' then s1(1)=':',end
    stk=list(sn(1)+rhsargs([s1(1),s2(1)]),'0')
  end
endfunction

function [stk,txt,top]=_g2sci()
// Generate code for |
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  stk=list(e1+' | '+e2,'2')
  top=top-1
endfunction

function [stk,txt,top]=_h2sci()
// Generate code for  &
// Copyright INRIA
// Author Serge Steer  
  s2=stk(top);s1=stk(top-1);
  txt=[]
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end
  stk=list(e1+' & '+e2,'2')
  top=top-1
endfunction

function [stk,txt,top]=_i2sci()
// Generate code for insertion
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  rhs=abs(evstr(op(3)))-2
  sto=stk(top);top=top-1
  sfrom=stk(top);top=top-1
  top=top-rhs+1
  s2=stk(top)
  if s2(1)=='eye()' then s2(1)=':',end
  if rhs==1 then
    if type(s2(1))==1 then // recursive extraction
      n=s2(1)
      ex=sto(1)
      for k=1:n
	ik=s2(k+1)
	if type(ik(1))==1 then
	  ex1=[]
	  for k1=1:ik(1),ex1=[ex1,ik(1+k1)(1)],end
	  ex=ex+rhsargs(ex1)
	else
	  if ik(2)=='0'&part(ik(1),1)=='''' then
	    ex=ex+'.'+part(ik(1),2:length(ik(1))-1)
	  else
	    if ik(1)=='eye()' then ik(1)=':',end  
	    ex=ex+'('+ik(1)+')'
	  end
	end
      end
      txt=ex+'='+sfrom(1)+';'
      stk=list(' ','-1')
    else
      if s2(1)=='eye()' then s2(1)=':',end  
      txt=sto(1)+'('+s2(1)+')='+sfrom(1)+';'
      stk=list(op(2),'-1') 
    end
  else // x(i,j)=y
    s1=stk(top+1)
    if s1(1)=='eye()' then s1(1)=':',end
    if s2(1)=='eye()' then s2(1)=':',end  
    txt=sto(1)+'('+s2(1)+','+s1(1)+') = '+sfrom(1)+';'
    stk=list(op(2),'-1')
  end
endfunction

function [stk,txt,top]=_imp2sci()
//code for 1:n
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  if op(3)=='2' then
    stk=list(stk(top-1)(1)+':'+stk(top)(1),'3')
    top=top-1
  else
    stk=list(stk(top-2)(1)+':'+stk(top-1)(1)+':'+stk(top)(1),'3')
    top=top-2
  end
endfunction

function [stk,txt,top]=_j2sci()
// Generates code for .^
// Copyright INRIA
// Author Serge Steer  

  s2=stk(top);s1=stk(top-1);
  txt=[]
  [ss1,te1]=s1(1:2);
  [ss2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then ss2='('+ss2+')',end
  if te1=='1'|te1=='2'|te1=='3' then ss1='('+ss1+')',end

  if part(ss2,1)=='-' then ss2='('+ss2+')',end
  stk=list(ss1+'.^'+ss2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_l2sci()
//  \
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);top=top-1;

  if s2(2)=='3'|s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
  if  s1(2)=='3'|s1(2)=='2' then s1(1)='('+s1(1)+')',end

  if part(s1(1),1)=='-' then s1(1)='('+s1(1)+')',end
  stk=list(s1(1)+'\'+s2(1),'1')
endfunction

function [stk,txt,top]=_q2sci()
//  .\
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);top=top-1;
  if s2(2)=='3'|s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
  if s1(2)=='3'|s1(2)=='2' then s1(1)='('+s1(1)+')',end

  stk=list(s1(1)+' .\ '+s2(1),'1')
endfunction

function [stk,txt,top]=_r2sci()
//  /
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top);s1=stk(top-1);top=top-1;

  if s2(2)=='3'|s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
  if s1(2)=='3'|s1(2)=='2' then s1(1)='('+s1(1)+')',end

  if part(s2(1),1)=='-' then s2(1)='('+s2(1)+')',end
  stk=list(s1(1)+'/'+s2(1),'1')
endfunction

function [stk,txt,top]=_s2sci()
// Generates code for substraction and sign change -
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top)
  if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
  if op(3)=='2' then
    s1=stk(top-1)
    if s1(2)=='3' then s1(1)='('+s1(1)+')',end
    stk=list(s1(1)+' - '+s2(1),'2')
    top=top-1
  else
    stk=list('-'+s2(1),'2')
  end
endfunction

function [stk,txt,top]=_t2sci()
// Generates code for transposition
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s2=stk(top)
  if s2(2)=='1'|s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
  stk=list(s2(1)+quote,s2(2))
endfunction

function [stk,txt,top]=_x2sci()
// Generates code for .*  
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' .* '+e2,'1')
  top=top-1
endfunction

function t=catcode(a,b)
// Copyright INRIA
// Author Serge Steer  

  if a==[] then 
    t=b
  elseif b==[]
    t=a
  else
    t=[a(1:$-1);a($)+b(1);b(2:$)]
  end
endfunction

function T=splitexp(t)
// Copyright INRIA
// Author Serge Steer  
  T=[]
  for i=1:size(t,'*')
    ti=t(i)
    ti=strsubst(ti,CR+';',';'+CR)
    ti=strsubst(ti,CR+', [',', ['+CR)
    ks=strindex(ti,CR)
    kd=1
    ind=''
    for kf=ks
      T=[T;ind+part(ti,kd:kf-1)];
      kd=kf+length(CR);
      ind='  ';
    end
    T=[T;ind+part(ti,kd:length(ti))]
  end
endfunction

function [stk,txt,top]=_u2sci()
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' *. '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_k2sci()
//generates code for .*.
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' .*. '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_v2sci()
//generates code for /.
// Copyright INRIA
// Author Serge Steer  
  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' /. '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_y2sci()
//generates code for ./.
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' ./. '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_z2sci()
//generates code for .\.
// Copyright INRIA
// Author Serge Steer  

  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' .\. '+e2,'1')
  top=top-1
endfunction

function [stk,txt,top]=_w2sci()
//generates code for .\
// Copyright INRIA
// Author Serge Steer  
  txt=[]
  s1=stk(top-1)
  s2=stk(top)
  [e1,te1]=s1(1:2);
  [e2,te2]=s2(1:2);
  //
  if te2=='1'|te2=='2'|te2=='3' then e2='('+e2+')',end
  if te1=='2'|te1=='3' then e1='('+e1+')',end

  stk=list(e1+' \. '+e2,'1')
  top=top-1
endfunction
