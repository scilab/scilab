function [value,ArrayName]=ReadmiMatrix(fd)
// Read a variable in a Matlab binary file
// This function has been developped following the 'MAT-File Format' description:
// www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 
// Copyright INRIA
// Authors: SS, VC

  [DataType,NumberOfBytes,Compressed]=ReadTag(fd);
  if meof(fd) then value=[],ArrayName="",return,end
  if DataType<>miMatrix then 
    error('Found Datatype='+string(DataType)+', expecting '+ ...
	  string(miMatrix))
  end
  if NumberOfBytes==0 then value=[],return,end
  [Flags,Class,NnzMax]=ReadArrayFlags(fd);
  DimensionArray=ReadDimensionArray(fd);
  ArrayName=ReadArrayName(fd)
  select Class
  case DoubleClass
    value=double(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case SingleClass
    value=ReadSimpleElement(fd,prod(DimensionArray),Class)
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Int8Class
    value=int8(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Uint8Class
    value=uint8(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Int16Class
    value=int16(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Uint16Class
    value=uint16(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Int32Class
    value=int32(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case Uint32Class
    value=uint32(ReadSimpleElement(fd,prod(DimensionArray),Class))
    if Flags(1) then 
      value=double(value)+%i*double(ReadSimpleElement(fd,prod(DimensionArray)))
    end
    value=matrix(value,DimensionArray)
  case CellClass 
   
    entries=list()
    for k=1:prod(DimensionArray)
      entries(k)=ReadmiMatrix(fd)
    end
    value=mlist(['ce','dims','entries'],int32(DimensionArray),entries)
  case CharClass 
    value=matrix(ReadSimpleElement(fd,prod(DimensionArray)),DimensionArray(1),-1)
    t=[];for v=value',t=[t;stripblanks(ascii(double(v)))];end
    value=t
  case StructClass 
    FieldNameLength=double(ReadSimpleElement(fd,1))
    FieldNames=matrix(ReadSimpleElement(fd),FieldNameLength,-1)
    NumberOfFields=size(FieldNames,2)
    Fnams=[];Fields=list();
    for k=1:NumberOfFields
      l=find(FieldNames(:,k)==0,1)-1;
      Fnams=[Fnams,stripblanks(ascii(double(FieldNames(1:l,k))))];
      Fields(k)=list();
    end

    if prod(DimensionArray)==1 then
       for k=1:NumberOfFields
	 Fields(k)=ReadmiMatrix(fd);
       end
    else
      Fk=list();for i=1:size(DimensionArray,'*'),Fk(i)=[];end
      for k=1:NumberOfFields,Fields(k)=Fk,end
      for i=1:prod(DimensionArray)
	for k=1:NumberOfFields
	  Fields(k)(i)=ReadmiMatrix(fd);
	end
      end
    end
    //Form Scilab representation
    value=mlist(['st' 'dims' Fnams],int32(DimensionArray),Fields(:))
  case ObjectClass 
    ClassName=stripblanks(ascii(double(ReadSimpleElement(fd))))
    FieldNameLength=double(ReadSimpleElement(fd,1))
    FieldNames=matrix(ReadSimpleElement(fd),FieldNameLength,-1)
    NumberOfFields=size(FieldNames,2)
    Fields=list();Fnams=[]
    for k=1:NumberOfFields
      l=find(FieldNames(:,k)==0,1)-1
      Fnams=[Fnams,stripblanks(ascii(double(FieldNames(1:l,k))))]
      Fields(k)=ReadmiMatrix(fd)
    end
    //Form Scilab representation
    value=tlist([ClassName, Fnams],Fields(:))
    select ClassName
    case 'inline' then
      value=Object2Inline(value)
    case 'ss' then
      value=Object2SS(value)
    case 'tf' then
      value=Object2tf(value)
    end
  case SparseClass then
    RowIndex=double(ReadSimpleElement(fd,NnzMax))
    ColumnIndex=double(ReadSimpleElement(fd,DimensionArray(2)+1))
    value=double(ReadSimpleElement(fd))
    if Flags(1) then 
      value=value+%i*double(ReadSimpleElement(fd))
    end
 
    //Form Scilab representation
    ptr=ColumnIndex(2:$)-ColumnIndex(1:$-1);
    col=[];cc=1;
    for ic=1:size(ptr,'*')
      col=[col;cc(ones(ptr(ic),1))];cc=cc+1;
    end
    //in some cases the initial value of ne is  bigger than necessary
    ne=min(size(RowIndex,'*'),size(col,'*'));
    RowIndex=RowIndex(1:ne);col=col(1:ne);
    if RowIndex<>[] then RowIndex=RowIndex(:)+1,end
    value=sparse([col(:),RowIndex],value(:),DimensionArray([2 1])).'
  else
    error('Unknown Class')
  end
endfunction

function [DataType,NumberOfBytes,Compressed]=ReadTag(fd)
 //--TAG
//Copyright INRIA   
//Author Serge Steer  
p1=mtell(fd) 

t=mget(2,md_s,fd);
if t==[] then //EOF
  DataType=0;NumberOfBytes=0,Compressed=%f
else
  if endian=='l' then t=t([2 1]),end
  Compressed=t(1)<>0;
  if Compressed then // compressed data element format
    NumberOfBytes=t(1)
    DataType=t(2)
  else
    mseek(p1,fd)
    DataType=mget(1,md_i,fd);
    NumberOfBytes=mget(1,md_i,fd);
  end 
end
endfunction



function [Flags,Class,NnzMax]=ReadArrayFlags(fd)
//Copyright INRIA
//Author Serge Steer    
  [DataType,NumberOfBytes,Compressed]=ReadTag(fd) 
  B=mget(4,'uc',fd);
  if endian=='l' then B=B([4 3 2 1]),end
  Class=B(4)
  Flags=byte2bits(B(3));Flags=Flags(4:-1:2)
  NnzMax=mget(1,md_i,fd)
endfunction

function dims=ReadDimensionArray(fd)
//Copyright INRIA  
//Author Serge Steer    
  dims=double(ReadSimpleElement(fd))
endfunction

function ArrayName=ReadArrayName(fd)
//Copyright INRIA
//Author Serge Steer    
  ArrayName=ascii(double(ReadSimpleElement(fd)))
endfunction

function value=ReadSimpleElement(fd,NumberOfValues,Class)
//Copyright INRIA  
//Author Serge Steer  
  pse=mtell(fd)
  [DataType,NumberOfBytes,Compressed]=ReadTag(fd) 
  select DataType
  case miDOUBLE
    if argn(2)==1 then NumberOfValues=NumberOfBytes/8,end
    value=mget(NumberOfValues,md_d,fd)
  case miSINGLE
    if argn(2)==1 then NumberOfValues=NumberOfBytes/4,end
    value=mget(NumberOfValues,md_f,fd)
  case miINT8
    if argn(2)==1 then NumberOfValues=NumberOfBytes,end
    value=mgeti(NumberOfValues,"c",fd)
  case miUINT8
    if argn(2)==1 then NumberOfValues=NumberOfBytes,end
    value=mgeti(NumberOfValues,"uc",fd)
   case miINT16
    if argn(2)==1 then NumberOfValues=NumberOfBytes/2,end
    value=mgeti(NumberOfValues,md_s,fd)
  case miUINT16
    if argn(2)==1 then NumberOfValues=NumberOfBytes/2,end
    value=mget(NumberOfValues,"u"+md_s,fd)
  case miUINT32
    if argn(2)==1 then NumberOfValues=NumberOfBytes/4,end
    value=mgeti(NumberOfValues,"u"+md_i,fd)
  case miINT32
    if argn(2)==1 then NumberOfValues=NumberOfBytes/4,end
    value=mgeti(NumberOfValues,md_i,fd)
  case miUINT64
    if argn(2)==1 then NumberOfValues=NumberOfBytes/8,end
    value=mget(NumberOfValues,"u"+md_l,fd)
  case miINT64
    if argn(2)==1 then NumberOfValues=NumberOfBytes/8,end
    value=mget(NumberOfValues,md_l,fd)
  case miMatrix
    mseek(pse,fd)
    [value,ArrayName]=ReadmiMatrix(fd)
  else
    disp("Not implemented DataType: "+string(DataType));
    pause
  end
  padding()

endfunction
  

function padding()
// skip padding data 
//----------------------------------------------
//Copyright INRIA
//Author Serge Steer  
  
//data fields are aligned on double words 
np=modulo(8-modulo(mtell(fd),8),8)
if np>0 then mget(np,'uc',fd),end
endfunction

function showbin(n,pi)
//for debugging purpose
//----------------------------------------------
//Copyright INRIA
//Author Serge Steer  
  
p=mtell(fd)
if argn(2)==2 then mseek(pi,fd),end
x=string(matrix(mgeti(8*n,'uc',fd),8,-1)')
t=emptystr(n,1)+'|'
for k=1:4
  t=t+part(x(:,k),1:max(length(x(:,k)))+1)
end
t=t+'|'
for k=5:8
  t=t+part(x(:,k),1:max(length(x(:,k)))+1)
end
t=t+'|'
write(%io(2),t,'(a)')
mseek(p,fd)
endfunction


function [head,version,swap]=matfile_header(fd)
//get the mat file header informations
//Copyright INRIA
//Author Serge Steer  
  
  head=ascii(mget(124,'uc',fd))
  version=mget(2,'uc',fd)
  //Magic number endian coding
  IM_MI=mget(2,'uc',fd);
  if and(IM_MI==[73,77]) then // little endian file
    swap='l'
  elseif and(IM_MI==[77,73]) then // big endian file
    swap='b'
  else
    mclose(fd);
    // This line has to be mofified according to message in 'loadmatfile' function
    error('Invalid level 5 binary MAT-file!') 
  end
endfunction

function LoadMatConstants()
//set constants. This function should be exec'ed
//Copyright INRIA
//Author Serge Steer  
  
  miINT8=1
  miUINT8=2
  miINT16=3
  miUINT16=4
  miINT32=5
  miUINT32=6
  miSINGLE=7
  //
  miDOUBLE=9
  //
  //
  miINT64=12
  miUINT64=13
  miMatrix=14

  CellClass=1
  StructClass=2
  ObjectClass=3
  CharClass=4
  SparseClass=5
  DoubleClass=6
  SingleClass=7
  Int8Class=8
  Uint8Class=9
  Int16Class=10
  Uint16Class=11
  Int32Class=12
  Uint32Class=13
  
  //--set various reading format
  md_i='i'+endian;md_d='d'+endian;md_s='s'+endian;md_l='l'+endian;md_f='f'+endian;

endfunction

function value=Object2Inline(value)
//convert inline object to scilab function
//Copyright INRIA
//Author Serge Steer  
  
  deff('ans=value('+strcat(stripblanks(value.args),',')+')',value.expr,'n')
  comp(value,1);code=macr2lst(value)
  load SCI/macros/m2sci/lib
  killed=[];quote='''';dquote="""";batch=%f
  [value,trad]=m2sci(code,'value',%f,%f)
  value($)='endfunction'
  //define the final version
  execstr(value)
endfunction

function res=Object2SS(res)
//convert ss object to scilab 'lss' 
//Copyright INRIA
//Author Serge Steer  
  A=res.a;if type(A)==17 then A=A.entries(1),end
  B=res.b;if type(B)==17 then B=B.entries(1),end
  C=res.c;if type(C)==17 then C=C.entries(1),end
  D=res.d;if type(D)==17 then D=D.entries(1),end
  E=res.e;if type(E)==17 then E=E.entries(1),end
  st_nam=res.StateName
  props=res.lti
  dt=props.Ts;if dt==0 then dt='c',end
  res=syslin(dt,A,B,C,D)
  res($+1)=props
  res(1)($+1)='Properties'
endfunction

function res=Object2tf(res)
//convert tf object to scilab 'r' 
//Copyright INRIA 
//Author Serge Steer  
  v=res.Variable
  dims=double(res.num.dims) //res.num.dims may be an integer array
  props=res.lti
  num=[];den=[];
  for k=1:prod(dims)
    num=[num;poly(res.num.entries(k)($:-1:1),v,'c')];
    den=[den;poly(res.den.entries(k)($:-1:1),v,'c')];
  end
  num=matrix(num,dims)
  den=matrix(den,dims)
  dt=props.Ts;if dt==0 then dt='c',end
  res=syslin(dt,num,den)
  res(1)($+1)='Properties'
  res($+1)=props
endfunction

function fd=open_matfile(fil)
//Copyright INRIA   
//Author Serge Steer  
  fil=stripblanks(fil)
  fd=mopen(fil,'rb',0)
endfunction

function b=int2bytes(i)
//Copyright INRIA   
//Author Serge Steer  
  it=inttype(i);it1=modulo(it,10)
  if it1==1 then
    b=i(:)
  else
    s=iconvert(2^(4*(0:it1-1)),it)
    d=i;b=s(:);
    for k=1:it1
      x=s(it1-k+1);b(k) = d/x; d = d-b(k)*x;
    end
  end
endfunction

function b=byte2bits(i)
//Copyright INRIA  
//Author Serge Steer  
  b=(iconvert(i,11)&iconvert(2^(0:3),11))<>uint8(0)
endfunction

function I=columnfirstorder(d)
  nd=size(d,'*')
  if nd==2 then
    I=matrix(matrix(1:prod(d),d)',1,-1)
  else
    dd=prod(d(3:$))
    I1=matrix(1:prod(d),d(1),d(2),dd)
    I=[]
    for k=1:dd
      I=[I matrix(I1(:,:,k)',1,-1)]
    end
  end
endfunction
