// Copyright ENPC/INRIA


deff('[n,a,b,c,d,e]=Fscanf(str,f)',['fd=mopen(TMPDIR+''/fscanf.rep'',''w'');';
	'mputstr(str,fd);mclose(fd);';
	'fd=mopen(TMPDIR+''/fscanf.rep'',''r'');';
	'[n,a,b,c,d,e]=mfscanf(fd,f);mclose(fd);']);


//--------------test format %i
[n,a]=Fscanf('123','%i');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%i');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2i');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5i');
//--------------test format %li
[n,a]=Fscanf('123','%li');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%li');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2li');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5li');
//--------------test format %hi
[n,a]=Fscanf('123','%hi');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%hi');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2hi');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5hi');
//--------------test format %d
[n,a]=Fscanf('123','%d');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%d');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2d');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5d');
//--------------test format %ld
[n,a]=Fscanf('123','%ld');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%ld');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2ld');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5ld');
//--------------test format %hd
[n,a]=Fscanf('123','%hd');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('     123','%hd');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf('123','%2hd');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123','%5hd');

//------------- test format %n 
// Note that %n returned values are not counted in n 
s1='123 45.67 pipo';s2=' foo';
[n,a,b,c,d,e]=Fscanf(s1+s2,'%d%lf%s%n%s');
if n<>4|a<>123|b<>45.67|c<>'pipo'|d<>length(s1)|e<>'foo' then pause,end

//------------- test format %u 
[n,a]=Fscanf('+123','%u');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf(' 123','%2u');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('+123','%5u');
if n<>1|a<>123 then pause,end
//------------- test format %lu 
[n,a]=Fscanf('+123','%lu');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf(' 123','%2lu');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('+123','%5lu');
if n<>1|a<>123 then pause,end
//------------- test format %hu 
[n,a]=Fscanf('+123','%hu');
if n<>1|a<>123 then pause,end
[n,a]=Fscanf(' 123','%2hu');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('+123','%5hu');
if n<>1|a<>123 then pause,end

//------------- test format %o
[n,a]=Fscanf('123','%o');
if n<>1|a<>83 then pause,end
[n,a]=Fscanf('     123','%o');
if n<>1|a<>83 then pause,end
[n,a]=Fscanf('123','%2o');
if n<>1|a<>10 then pause,end
[n,a]=Fscanf('123','%5o');
if n<>1|a<>83 then pause,end

//------------- test format %x
rep=10*16^2+11*16+12;
[n,a]=Fscanf('0xabc','%x');
if n<>1|a<>rep then pause,end
[n,a]=Fscanf('     0xabc','%x');
if n<>1|a<>rep then pause,end
[n,a]=Fscanf('abc','%2x');
if n<>1|a<>10*16+11 then pause,end
[n,a]=Fscanf('0xabc','%5x');
if n<>1|a<>rep then pause,end

//------------- test format %e
[n,a]=Fscanf('123.45','%e');
if n<>1|norm(a-123.45)>1.e-2 then pause,end
[n,a]=Fscanf('     123.45','%e');
if n<>1|norm(a-123.45)>1.e-2 then pause,end
[n,a]=Fscanf('123.45','%2e');
if n<>1| norm(a-12)>0.1 then pause,end
[n,a]=Fscanf('123.45','%5e');
if n<>1|norm(a-123.4)>1.e-1 then pause,end

//------------- test format %le
[n,a]=Fscanf('123.45','%le');
if n<>1|a<>123.45 then pause,end
[n,a]=Fscanf('     123.45','%le');
if n<>1|a<>123.45 then pause,end
[n,a]=Fscanf('123.45','%2le');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123.45','%5le');
if n<>1|a<>123.4 then pause,end

//------------- test format %f
[n,a]=Fscanf('123.45','%f');
if n<>1|norm(a-123.45)>1.e-2 then pause,end
[n,a]=Fscanf('     123.45','%f');
if n<>1|norm(a-123.45)>1.e-2 then pause,end
[n,a]=Fscanf('123.45','%2f');
if n<>1| norm(a-12)>0.1 then pause,end
[n,a]=Fscanf('123.45','%5f');
if n<>1|norm(a-123.4)>1.e-1 then pause,end

//------------- test format %lf
[n,a]=Fscanf('123.45','%lf');
if n<>1|a<>123.45 then pause,end
[n,a]=Fscanf('     123.45','%lf');
if n<>1|a<>123.45 then pause,end
[n,a]=Fscanf('123.45','%2lf');
if n<>1|a<>12 then pause,end
[n,a]=Fscanf('123.45','%5lf');
if n<>1|a<>123.4 then pause,end

//------------- test format %s
[n,a]=Fscanf('123','%s');
if n<>1|a<>'123' then pause,end
[n,a]=Fscanf('     123','%s');
if n<>1|a<>'123' then pause,end
[n,a]=Fscanf('123','%2s');
if n<>1|a<>'12' then pause,end
[n,a]=Fscanf('123','%5s');
if n<>1|a<>'123' then pause,end

//------------- test format %c
//XXXX : attention a finir .... il faut ? scaner \n comme 
//un seul caractere  
[n,a,b,c,d]=Fscanf(' 12\n','%c%c%c%c');
if n<>4|a<>' '|b<>'1'|c<>'2' then pause,end

//------------- test format [
[n,a,b]=Fscanf('012345abczoo','%[0-9abc]%s');
if n<>2|a<>'012345abc'|b<>'zoo' then pause,end
//------------- test ignoring arguments 
[n,a,b]=msscanf('123 4 pipo poo','%*s%s%*s%s');
//if n<>2|a<>'4'|b<>'poo' then pause,end
//------------- test composed directives 
[n,a]=Fscanf('123 4','123%le');
if n<>1|a<>4 then pause,end
[n,a,b,c]=Fscanf('xxxxx 4 test 23.45','xxxxx%d%s%le');
if n<>3|a<>4|b<>'test'|c<>23.45 then pause,end
[n,a,b]=Fscanf('123 456','%le%le');
if n<>2|a<>123|b<>456 then pause,end

//------------- test mismatch 
[n,a]=Fscanf('123 poo','123%le');
if n<>0 then pause,end

//------------- test end-of-file
[n,a]=Fscanf('123','123%le');
if n<>-1 then pause,end


