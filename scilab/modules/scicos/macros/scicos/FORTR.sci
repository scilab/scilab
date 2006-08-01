function [ok,tt]=FORTR(funam,tt,inp,out)
//
ni=size(inp,'*')
no=size(out,'*')
if tt==[] then

  tete1=['      subroutine '+funam+'(flag,nevprt,t,xd,x,nx,z,nz,tvec,';..
      '     $        ntvec,rpar,nrpar,ipar,nipar']

  tete2= '     $        '
  for i=1:ni
    tete2=tete2+',u'+string(i)+',nu'+string(i)
  end
  for i=1:no
    tete2=tete2+',y'+string(i)+',ny'+string(i)
  end
  tete2=tete2+')'

  tete3=['      double precision t,xd(*),x(*),z(*),tvec(*)';..
    '      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)']

  tete4= '      double precision rpar(*)'
    for i=1:ni
      tete4=tete4+',u'+string(i)+'(*)'
    end
    for i=1:no
      tete4=tete4+',y'+string(i)+'(*)'
    end
    tetec=['c';'c'];tetev=[' ';' '];
    tetend='      end'
    
    textmp=[tete1;tete2;tetec;tete3;tete4;tetec;tetev;tetec;tetend];
  else
    textmp=tt;
  end
  
  while 1==1
      [txt]=x_dialog(['Function definition in fortran';
	'Here is a skeleton of the functions which you shoud edit'],..
	 textmp);

      if txt<>[] then
	tt=txt
	[ok]=scicos_block_link(funam,tt,'f')
	if ok then
	  textmp=txt;
	end
	break;
      else
	ok=%f;break;
      end  
  end
    
endfunction
