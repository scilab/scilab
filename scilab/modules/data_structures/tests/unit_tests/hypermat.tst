//extraction
Data=list(1:12,(1:12)+rand(1,12)*%i,int32(1:12),(1:12)+%s,rand(1,12)>0.5,string(1:12));

test=1;
msg='Problem with extraction for hypermatrix of type %s in test %d.%d \n';
for k=1:size(Data)
  data=Data(k);td=typeof(data);
  h=hypermat([2 3 2],data);
  if h(2,3,2)<>data(12) then mprintf(msg,td,test,k); pause,end
  if or(h(2,:,2)<>data([8 10 12])) then mprintf(msg,td,test,k); pause,end
  if or(h($,:,2)<>data([8 10 12])) then mprintf(msg,td,test,k); pause,end
  if or(h(:,3,2)<>matrix(data([11 12]),-1,1)) then mprintf(msg,test,k); pause,end
  if or(h(1:2)<>matrix(data([1 2]),-1,1)) then mprintf(msg,td,test,k); pause,end
  if or(h(1:3)<>matrix(data([1 2 3]),-1,1)) then mprintf(msg,td,test,k); pause,end
  if or(h(1,2:3)<>data([3 5])) then mprintf(msg,td,test,k); pause,end
  if or(h(1,4:5)<>data([7 9])) then mprintf(msg,td,test,k); pause,end
  msgToPrint=msprintf('    test%d.%d completed\n',test,k);write(%io(2),msgToPrint);
end
//insertion of []
test=2;
for k=1:size(Data)
  data=Data(k);td=typeof(data);
  h=hypermat([2 3 2],data);h1=h;
  h(2,:,:)=[];
  if or(size(h)<>[1 3 2]) then mprintf(msg,td,test,k); pause,end
  if or(h<>h1(1,:,:))  then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);h1=h;
  h(:,1:2,:)=[];
  if or(size(h)<>[2 1 2]) then mprintf(msg,td,test,k); pause,end
  if or(h<>h1(:,3,:))  then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h(:,:,:)=[];
  if h<>[] then mprintf(msg,td,test,k); pause,end
  msgToPrint=msprintf('    test%d.%d completed\n',test,k);write(%io(2),msgToPrint);

end


msg='Problem with insertion for hypermatrix of type %s in test %d.%d \n';  
//insertion of a single element
test=3;
for k=1:size(Data)
  data=Data(k);td=typeof(data);
  I=data(1);
  h=hypermat([2 3 2],data);
  h(2,3,2)=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(2,3,2)<>I then mprintf(msg,td,test,k); pause,end
  
  h=hypermat([2 3 2],data);
  h(2,3)=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(2,3,1)<>I then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h(2)=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(2,1,1)<>I then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h(3)=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(1,2,1)<>I then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h(3)=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(1,2,1)<>I then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h([3,7])=I;
  if or(size(h)<>[2 3 2]) then mprintf(msg,td,test,k); pause,end
  if h(1,2,1)<>I|h(1,1,2)<>I then mprintf(msg,td,test,k); pause,end

  h=hypermat([2 3 2],data);
  h(13:15)=I;
  if type(h)<>type(data) then mprintf(msg,td,test,k); pause,end
  if or(size(h)<>[15,1]) then mprintf(msg,td,test,k); pause,end
  if or(h(13:15)<>I) then mprintf(msg,td,test,k); pause,end
  msgToPrint=msprintf('    test%d.%d completed\n',test,k);write(%io(2),msgToPrint);

end

//extension of a 2D matrix to a 3D one
test=4;
for k=1:size(Data)
  data=matrix(Data(k),3,-1);td=typeof(data);
  I=data(1,1);
  h=data;
  h(3,4,2)=I;
  if or(size(h)<>[3 4 2]) then mprintf(msg,td,test,k); pause,end
  if h(:,:,1)<>data then mprintf(msg,td,test,k); pause,end
  d=[];d(3,4)=I;
  if h(:,:,2)<>d then mprintf(msg,td,test,k); pause,end
  msgToPrint=msprintf('    test%d.%d completed\n',test,k);write(%io(2),msgToPrint);
end



