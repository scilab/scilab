function titlepage(str,win)
//titlepage(str) genere une page de titres graphiques contenant la ma-
//trice de chaines de caracteres str ecrite centree dans  la page avec
//une taille de caractere maximale.
//!
//origine S Steer INRIA 1989
// Copyright INRIA
[lhs,rhs]=argn(0)
old=xget('window')
if rhs==2 then xset('window',win);end
plot2d([0,1],[0,1],[-1,-1],"022");
xstringb(0,0,str,1,1,'fill');
xset('window',old)
endfunction
