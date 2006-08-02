function y=asciimat(x)
// Copyright INRIA
// This function converts a matrix of string into a matrix of ascii codes using ascii() Scilab function
// and converts a array of ascii codes into a array of string 
// Returned value have same size as input value instead of second dims !
// Fonction created because ascii() Scilab function returns a row vector
if size(size(x),"*")<=2 then
  if type(x)==10 then
    if x==[] then
      y=[]
      return
    else 
      ytemp=[];
      for i=1:size(x,1)
        ytemp=[ytemp ascii(x(i,:))]
      end
      nbcol= size(ytemp,'*')*size(x,2)/size(x,'*') 
      if nbcol-floor(nbcol)<>0 then
        error("input argument: inconsistent size ")
      end
      y=[]
      for i=1:size(x,1)
	y=[y ;ytemp(1+(i-1)*nbcol:i*nbcol)]
      end   
  end
  elseif type(x)==1 | type(x)==8 then
    y=[]
    for k=1:size(x,1)
      y(k)=ascii(x(k,:))
    end
  else
    error("Not yet implemented")
  end
elseif size(size(x),"*")>2 then
  if typeof(x)=="hypermat" then
    if type(x.entries)==1 | type(x.entries)==8
      n=size(size(x),"*")
      dims=size(x)
      dims=[dims(1),dims(3:n)]
      V=' '
      D=prod(dims)
      DD=D/size(x,1)
      V=part(V,ones(1,size(x,2)))
      V=V+emptystr(D,1)
      for l=1:DD
	V((l-1)*size(x,1)+1:l*size(x,1))=asciimat(x(:,:,l))
      end
      y=mlist(['hm','dims','entries'],int32([dims]),V)
    end
  end
else
  error("Not yet implemented")
end
endfunction
