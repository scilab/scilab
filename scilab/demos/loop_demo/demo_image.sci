function demo_image()

demo_help demo_image
xdel() ;

// read image
[mat,h] = readppm('./scilab_logo.ppm');

// get three matrixes with the three channels
[r,g,b] = col2rgb(mat) ;

// display the initial image
[RGB,mat] = ppm2sci(mat,'p') ;
drawlater();
imageview(RGB,mat) ;
SetPosition_Small();
drawnow();


// compute the intensity matrix
intens = double( r + g + b ) ;

// fill the edges matrix
// and compute the intensity histogramm
[n,m] = size(intens) ;
edges = ones(n,m) ;

// 16 baquets
nb_baquet   = 32 ;
size_3      = 3 / 256 ;
inv_size    = 1 / 256 ;
histo       = zeros(1,nb_baquet) ;



for i = 2:n-1,
  for j= 2:m-1,
    xDiff = intens( i+1, j   ) - intens( i-1, j   ) ;
    yDiff = intens( i,   j+1 ) - intens( i  , j-1 ) ;

    // get the number of the baquet
    histo_color = ( intens(i,j) * nb_baquet ) / 256 + 1 ;
    histo( histo_color ) = histo( histo_color ) + 1 ;

    //edges(i,j) = 0.25 * sqrt( xDiff * xDiff + yDiff * yDiff ) ;
    edges(i,j) = 0.5 * ( abs( xDiff ) + abs( yDiff ) ) ;
  end
end

// display the histogramm
drawlater() ;
scf();
x = 0:256/nb_baquet:255;
//xtitle('Image intensity histogram');

// put histo between 0 and 1
histo = histo / ( n*m ) ;
bar( x, histo ) ;

SetPosition() ;
a = gca() ;
a.auto_ticks = ['on','on','on'] ;
t = a.title ;
t.text = 'Image intensity histogram' ;
t.font_size = 5 ;
SetPosition() ;
drawnow() ;

realtimeinit(1.0)
for i=1:10,
  realtime(i);
end
xdel() ;

// dipslay a pie with the three colors channels
drawlater() ;
scf();
red_sum   = double( sum(r) ) ;
green_sum = double( sum(g) ) ;
blue_sum  = double( sum(b) ) ;

sums = [green_sum,red_sum,blue_sum];
disp = [0 0 0];
//txt = ["blue","green","red"];
//xtitle('Proportion of the red, green and blue channels in the image');
pie( sums, disp ) ;
a = gca() ;
t = a.title ;
t.text = 'Proportion of the red, green and blue channels in the image' ;
t.font_size = 5 ;
SetPosition() ;
drawnow() ;

realtimeinit(1.0)
for i=1:10,
  realtime(i);
end
xdel() ;

res = uint8(edges);

// convert back thre three channels in one matrix
mat = rgb2col(res,res,res) ;

// dislay the result image
[RGB,mat] = ppm2sci(mat,'p') ;
xdel();
drawlater();
imageview(RGB,mat);
SetPosition_Small();
drawnow();


realtimeinit(1.0)
for i=1:10,
  realtime(i);
end

xdel() ;

endfunction

function imageview(RGB,m)
[M,N]=size(m)
xset('colormap',RGB)
xsetech([-0.16 -0.16 1.32 1.32])
xset('wdim',M,N)
Matplot(m','020')
endfunction

function [RGB,m]=ppm2sci(m,flag)
[nr,nc]=size(m);
// this function can be improved by the use of short integers but this
// require a sort for short ints
if flag=='p' then // color
  //get the colormap
  disp('Warning, this may take a while, be patient please...')
  k=1:3:nr;
  C=double(m(k,:))+double(m(k+1,:))*256+double(m(k+2,:))*(65536);
  cs=sort(C);k2=find(cs(2:$)<>cs(1:$-1));
  rgb=cs([k2(1) k2+1]);cs=[];rgb=rgb(:)

  R=modulo(rgb,256);
  w=(rgb-R)/256;G=modulo(w,256);B=(w-G)/256;w=[]
  RGB=[R G B]/256;R=[];G=[];B=[];

  m=zeros(nr/3,nc);
  for l=1:size(rgb,'*')
    m(find(C==rgb(l)))=l
  end
else //gray
  m=double(m)+1
  RGB=graycolormap(max(m)+1)
end
endfunction

function [m,head]=readimage(fil)
// this function reads images in PBM, PGM, PPM (ascii) formats
// fil is a string containing the file name containing the image
head=[]
lignecourante=[]
m=[]
//this command adds a carriage return to the end of the file
u=file('open',fil,'old')
file('close',u)
host('echo "'"' >> '+fil)
u=file('open',fil,'old')
//errcatch(-1,'continue')
head=read(u,1,1,'(a)')
niveau=1;
if head<>'P1'&head<>'P2'&head<>'P3' then
  error('Not a PBM/PGM/PPM file')
end
if head=='P1' then  niveau=0; end
if head=='P3' then niveau=2;end
c='#';


while c=='#',
  lignecourante=read(u,1,1,'(a)');
  head=[head;lignecourante];
  c=sscanf(lignecourante,'%c');
end;
[sz1,sz2]=sscanf(lignecourante,'%d%d');
if niveau~=0 then
  head=[head;read(u,1,1,'(a)')];
end;

if niveau==2 then
  m=matrix(read(u,1,3*sz1*sz2),3*sz1,sz2);
else
  m=matrix(read(u,1,sz1*sz2),sz1,sz2);
end;

file('close',u)
endfunction


function [r,g,b]=col2rgb(m)
//this function converts m, the result of reading a ppm color image
//into 3 matrices coding the red, green and blue components.
sz=size(m);
r=zeros(sz(1)/3,sz(2));
g=zeros(sz(1)/3,sz(2));
b=zeros(sz(1)/3,sz(2));
k=1:3:sz(1)
r=m(k,:);g=m(k+1,:);b=m(k+2,:);
endfunction

function [q]=rgb2col(r,g,b)
//this function constructs a matrix m coding a color image from
//3 matrices coding the red, green and blue components.
sz=size(r);
k=1:3:3*sz(1);
q=zeros(3*sz(1),sz(2));
q(k,:)=r;q(k+1,:)=g;q(k+2,:)=b;
endfunction

function [m,head]=readppm(fil)
// this function reads images in PBM, PGM, PPM (ascii and binary) formats
// fil is a string containing the file name containing the image
head=[]
lignecourante=[]
m=[]
[u,err]=mopen(fil,'rb')
if err<>0 then error('Impossible to open file '+fil),end
// read the type
head=getimgline(u)
h=head
if h~='P'+string([1 2 3 4 5 6]) then
  error('Not a PBM/PGM/PPM file')
end
ftyp=evstr(part(h,2))


//comments
c='#';
while c=='#',
  lignecourante=getimgline(u)
  head=[head;lignecourante];
  c=part(lignecourante,1)
end;
//image size
execstr('sz=['+lignecourante+']')
// number of colors

if and(ftyp<>[1 4]) then
  execstr('nc='+getimgline(u))
else
  nc=2
end;
if or(ftyp==[3 6]) then
  nv=3*sz(1)*sz(2),
  m=uint8(0);m(3*sz(1),sz(2))=uint8(0);
else
  nv=sz(1)*sz(2),
  m=uint8(0);m(sz(1),sz(2))=uint8(0);
end

l=1
if or(ftyp==[1 2 3]) then //ascii files
  mclose(u)
  u=file('open',fil,'old')
  w=read(u,size(head,1),1,'(a)');
  select ftyp
  case 1 then step=35*500;
  case 2 then step=17*1000;
  case 3 then step=15*1000;
  end
  nr=int(nv/step)

  errcatch(62,'continue','nomessage')
  for k=1:nr
    m(l:l+step-1)=uint8(read(u,1,step))
    if iserror(62)<>0 then
      errcatch(-1);errclear(-1)
      pause
      error('End of file reached, check for carriage return at the file end')
    end
    l=l+step
  end
  if nr*step<nv then
    r=nv-nr*step
    m(l:nv)=uint8(read(u,1,nv-l+1))
    if iserror(62)<>0 then
      errcatch(-1);errclear(-1)
      pause
      error('End of file reached, check for carriage return at the file end')
    end
    l=l+r
  end
  errcatch(-1)
  file('close',u)
else
  if ftyp<>4 then // uchar stored
    step=10000
    nr=ceil(nv/step)
    for k=1:nr
      tmp=mget(step,'uc',u);nt=size(tmp,'*')
      if tmp==[] then pause,end
      m(l:l+nt-1)=uint8(tmp);l=l+nt
    end
  else //bits are stored
    error('Binary Black and White  files cannot be read yet')
    step=1000
    nr=ceil(nv/(8*step))
    f=uint8(2.^(0:7))
    for k=1:nr
      tmp=uint8(mget(step,'uc',u));
      if tmp==[] then pause,end
      tmp=matrix([tmp&f(1);
	   (tmp&f(2));
	   (tmp&f(3));
	   (tmp&f(4));
	   (tmp&f(5));
           (tmp&f(6));
	   (tmp&f(7));
	   (tmp&f(8))],1,-1);
       tmp(find(tmp>uint8(1)))=uint8(1) //ne marche pas
       nt=size(tmp,'*')
       m(l:l+nt-1)=tmp
      l=l+nt;
    end
    m(find(m>uint8(1)))=uint8(1)

  end
  mclose(u)
end

endfunction


function l=getimgline(u)
h=[]
while %t
 c=mget(1,'uc',u)
 if c==10 then break,end
 h=[h c]
end

//
// Correcting BUG : Vince & Titiii
// We have to take care about the file Format !
// We do not care about the OS *<8-)
//
if h($) == 13 then
  s = size(h);
  s = s(2) - 1 ;
  l = ascii(h(1:s)) ;
else
  l = ascii(h) ;
end

endfunction
