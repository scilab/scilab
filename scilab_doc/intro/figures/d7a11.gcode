//some examples
str_l=list();
//
str_l(1)=['plot3d1();';
      'title=[''plot3d1 : z=sin(x)*cos(y)''];';
      'xtitle(title,'' '','' '');'];
//
str_l(2)=['contour();';
      'title=[''contour ''];';
      'xtitle(title,'' '','' '');'];
//
str_l(3)=['champ();';
      'title=[''champ ''];';
      'xtitle(title,'' '','' '');'];
//
str_l(4)=['t=%pi*(-10:10)/10;';
          'deff(''[z]=surf(x,y)'',''z=sin(x)*cos(y)'');';
          'rect=[-%pi,%pi,-%pi,%pi,-5,1];';
          'z=feval(t,t,surf);';
          'contour(t,t,z,10,35,45,''X@Y@Z'',[1,1,0],rect,-5);';
          'plot3d(t,t,z,35,45,''X@Y@Z'',[2,1,3],rect);';
          'title=[''plot3d and contour ''];';
          'xtitle(title,'' '','' '');'];
//
for i=1:4,xinit('d7a11.ps'+string(i)');
          execstr(str_l(i)),xend();end
