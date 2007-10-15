function [ok,tt]=CFORTR(funam,tt,inp,out)
// Copyright INRIA
//
ni=size(inp,'*')
no=size(out,'*')
if tt==[] then

  tete1=['#include <machine.h>';'#include <math.h>';...
	  'void '+funam+'(flag,nevprt,t,xd,x,nx,z,nz,tvec,';..
      '             ntvec,rpar,nrpar,ipar,nipar']

  tete2= '      '
  decl=''
  for i=1:ni
    tete2=tete2+',u'+string(i)+',nu'+string(i)
    decl=decl+',*nu'+string(i)
  end
  for i=1:no
    tete2=tete2+',y'+string(i)+',ny'+string(i)
    decl=decl+',*ny'+string(i)
  end
  tete2=tete2+')'

  tete3=['      double *t,xd[],x[],z[],tvec[];';..
    '      integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar'+decl+';']


  tete4= '      double rpar[]'
    for i=1:ni
      tete4=tete4+',u'+string(i)+'[]'
    end
    for i=1:no
      tete4=tete4+',y'+string(i)+'[]'
    end
    tetev=[' ';' ']
    
  textmp=[tete1;tete2;tetev;tete3;tete4+';';
          '/* modify below this line */';
	  '{'
	  '}'
	tetev];
  else
    textmp=tt;
  end
  
  while 1==1
      [txt]=x_dialog(['Function definition in C';
	'Here is a skeleton of the functions which you shoud edit'],..
	 textmp);

      if txt<>[] then
	tt=txt
	[ok]=scicos_block_link(funam,tt,'c')
	if ok then
	  textmp=txt;
	end
	break;
      else
	ok=%f;break;
      end  
  end
    
  
endfunction
