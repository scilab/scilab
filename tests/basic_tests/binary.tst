//---------------- first test 
  filen = 'test.bin';
  mopen(filen,'wb');
  mput(1996,'l');mput(1996,'i');mput(1996,'s');mput(98,'c');
  mput(1996,'ul');mput(1996,'ui');mput(1996,'us');mput(98,'uc');
  mput(1996,'d');mput(1996,'f');
// force little-endian 
  mput(1996,'ll');mput(1996,'il');mput(1996,'sl');mput(98,'cl');
  mput(1996,'ull');mput(1996,'uil');mput(1996,'usl');mput(98,'ucl');
  mput(1996,'dl');mput(1996,'fl');
// force big-endian 
  mput(1996,'lb');mput(1996,'ib');mput(1996,'sb');mput(98,'cb');
  mput(1996,'ulb');mput(1996,'uib');mput(1996,'usb');mput(98,'ucb');
  mput(1996,'db');mput(1996,'fb');
//
  mputstr('Scilab');
  mclose();
  mopen(filen,'rb');
  if 1996<>mget(1,'l') then pause,end
  if 1996<>mget(1,'i') then pause,end
  if 1996<>mget(1,'s') then pause,end
  if   98<>mget(1,'c') then pause,end
  if 1996<>mget(1,'ul') then pause,end
  if 1996<>mget(1,'ui') then pause,end
  if 1996<>mget(1,'us') then pause,end
  if   98<>mget(1,'uc') then pause,end
  if 1996<>mget(1,'d') then pause,end
  if 1996<>mget(1,'f') then pause,end
  // force little-endian
  if 1996<>mget(1,'ll') then pause,end
  if 1996<>mget(1,'il') then pause,end
  if 1996<>mget(1,'sl') then pause,end
  if   98<>mget(1,'cl') then pause,end
  if 1996<>mget(1,'ull') then pause,end
  if 1996<>mget(1,'uil') then pause,end
  if 1996<>mget(1,'usl') then pause,end
  if   98<>mget(1,'ucl') then pause,end
  if 1996<>mget(1,'dl') then pause,end
  if 1996<>mget(1,'fl') then pause,end
  // force big-endian 
  if 1996<>mget(1,'lb') then pause,end
  if 1996<>mget(1,'ib') then pause,end
  if 1996<>mget(1,'sb') then pause,end
  if   98<>mget(1,'cb') then pause,end
  if 1996<>mget(1,'ulb') then pause,end
  if 1996<>mget(1,'uib') then pause,end
  if 1996<>mget(1,'usb') then pause,end
  if   98<>mget(1,'ucb') then pause,end
  if 1996<>mget(1,'db') then pause,end
  if 1996<>mget(1,'fb') then pause,end
  //
  if 'Scilab'<>mgetstr(6) then pause,end
  mclose();

//---------------- an example with two files

  file1 = 'test1.bin';
  file2 = 'test2.bin';
  fd1=mopen(file1,'wb');
  fd2=mopen(file2,'wb');
  mput(1996,'ull',fd1);
  mput(1996,'ull',fd2);
  mclose(fd1);
  mclose(fd2);

  fd1=mopen(file1,'rb');
  if 1996<>mget(1,'ull',fd1) then pause,end
  fd2=mopen(file2,'rb');
  if 1996<>mget(1,'ull',fd2) then pause,end
  mclose(fd1);
  mclose(fd2);

//------------------ and example with mseek
  file3='test3.bin';
  fd1= mopen(file3,'wb');
  for i=1:10, mput(i,'d'); end
  mseek(0);
  mput(678,'d');
  mseek(0,fd1,'end');
  mput(932,'d');
  mclose(fd1);
  fd1= mopen(file3,'rb');
  res=mget(11,'d');
  res1=[1:11]; res1(1)=678;res1($)=932;
  if res1<>res then pause,end
  mseek(0,fd1,'set');
  // trying to read more than stored data
  res1=mget(100,'d',fd1);
  if res1<>res then pause,end
  [a]=meof(fd1);
  if a==0 then pause,end
  mclearerr(fd1)
  [a]= mclose(fd1);
  if a<>0 then pause,end
//------------------ examples with arrays 
  filen = 'test.bin';
  mopen(filen,'wb');
  mput(1:100,'l');
  mclose();
  mopen(filen,'rb');
  X=mget(200,'l');
  mclose();
  if size(X,'*')<> 100 then pause,end
  if norm(X-(1:100))> 10*%eps then pause,end

  filen = 'test.bin';
  mopen(filen,'wb');
  mput(1:100,'c');
  mclose();
  mopen(filen,'rb');
  X=mget(200,'c');
  mclose();
  if size(X,'*')<> 100 then pause,end
  if norm(X-(1:100))> 10*%eps then pause,end

