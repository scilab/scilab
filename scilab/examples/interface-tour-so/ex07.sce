e1=rand(4,3);
e2=int(10*rand(5,6));
e3='ex07c';
e4=rand(7,8) >= 0;
e5=string(1:10);
e6=int(10*sprand(10,10,0.1));
e7=int8(10*rand(1,5,'u'));

[x1,x2,x3,x4,x5,x6,x7]=ex7c_1(list(e1,e2,e3,e4,e5,e6,e7));

if x1<>e1  then pause,end
if x2<>e2  then pause,end
if x3<>e3  then pause,end
if x4<>e4  then pause,end
if x5<>e5  then pause,end
if x6<>e6  then pause,end
if x7<>e7  then pause,end

e6 = e6 +%i*int(10*sprand(10,10,0.1));

[x1,x2,x3,x4,x5,x6,x7]=ex7c_1(list(e1,e2,e3,e4,e5,e6,e7));

if x1<>e1  then pause,end
if x2<>e2  then pause,end
if x3<>e3  then pause,end
if x4<>e4  then pause,end
if x5<>e5  then pause,end
if x6<>e6  then pause,end
if x7<>e7  then pause,end

//-------------------

L= ex7c_2([10,20,30],[1:10],"string",[%t,%f],int16([1,2,90]));
if L(1)<>[10,20,30]  then pause,end
if L(2)<>[1:10]  then pause,end
if L(3)<>"string"  then pause,end
if L(4)<>[%t,%f]  then pause,end
if L(5)<>int16([1,2,90])  then pause,end

//----------------------

L=ex7c_3('l');
if L(1)<>['type','x1','x2','x3','x4','x4'] then pause,end
if L(2)<>[10,20,30]  then pause,end
if L(3)<>[1:4]  then pause,end
if L(4)<>"a string"  then pause,end
if L(5)<>[%t,%f]  then pause,end
Wres=sparse([2,2,3,3,3,4,5;1,2,1,2,3,3,5]',[1,2,4,5,3,6,7]);
if L(6)<>Wres then pause,end
Ires=uint8([41,52,63]);
if L(7)<>Ires then pause,end

L=ex7c_3('t');
if typeof(L)<>'type'  then pause,end
if L.x1 <>[10,20,30]  then pause,end

L=ex7c_3('m');
if typeof(L)<>'type'  then pause,end
if L.x1 <>[10,20,30]  then pause,end













