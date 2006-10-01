// icse.init.bas : initialisation et tests pour icse
//**************************************************
//
//
// creation y0 (etat initial)
// Copyright INRIA
if exists('y0')==0, y0=ones(1,ny); end;
// creation de b,fy et fu et passage en vecteur ligne
if exists('b')==0, b=ones(1,ny); end;
if exists('fy')==0, fy1=ones(1,ny*ny);
     else fy1=matrix(fy,1,ny*ny) ; end;
if exists('fu')==0, fu1=ones(1,ny*(nuc+nuv));
   else fu1=matrix(fu,1,ny*(nuc+nuv)); end;
//
format('e');
iu(5)=0;    
[xx,nitu]=size(itu);    
[yy,ndtu]=size(dtu);    
if xx+yy>2, error('itu et/ou dtu n'est pas un vecteur ligne'), end;     
u=[]; if nuc>0, u=uc; end;    
if nuv>0, u=[u,uv]; end;
nu=nuc+nuv*(nti+ntf+1);   
if size(u)<>[1,nu],error('dimensions du controle incompatibles'),end;   
clear xx yy;
//    initialisation du common icsez
[nitv,nrtv,ndtv]=fort('icse0',nu,1,'i',t0,2,'d',tf,3,'d',dti,4,'d',..   
dtf,5,'d',ermx,6,'d',iu,7,'i',nuc,8,'i',nuv,9,'i',ilin,10,'i',nti,..    
11,'i',ntf,12,'i',ny,13,'i',nea,14,'i',itmx,15,'i',nex,16,'i',nob,..    
17,'i',ntob,18,'i',ntobi,19,'i',nitu,20,'i',ndtu,21,'i','sort',.. 
[1,1],22,'i',[1,1],23,'i',[1,1],24,'i');  
