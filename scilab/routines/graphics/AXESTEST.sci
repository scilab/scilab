// dir = 'u','r','d','l'  [default -> 'l' ] 
// fontsize =             [default -> -1   ] 
// format_n = format to use for numbers (unused if strings) 
// seg = flag 1 or 0 draw the base segment of the axis (default 1)
// sub_int  = number of sub tics (default 2) 
// textcolor =            [default -> -1 ]
// ticscolor =            [default -> -1 ]
// tics = 'v' 'r' 'i'     [default -> 'v' ] 
//        gives tics type : vector | range | irange (v,r,i) 
// val  = string matrix 
// x =  scalar | vecteur | range | irange 
// y =  scalar | vecteur | range | irange 
// 
// constraints : 
// ------------
//   dir = 'u' | 'd' ==> y= scalar | [] 
//                       x= vecteur | range | irange 
//   dir = 'r' | 'l' ==> x= scalar | [] 
//                       y= vecteur | range | irange 
//   tics = 'r'          ==> x or y is of size 3 (according to dir)
//   tics = 'i'          ==> x or y is of size 4
//   val  =              ==> must be of size compatible with x or y 
//                       according to dir 
// Finir et mettre ds le manuel XXXXX 
// question faut-il 2 fonctions ou une ?

plot2d(1:10,1:10,1,"020")
xaxis(x=2:7,y=4,dir='u',tics='v') // ok 
xaxis(x=2:7,dir='u',tics='v') 
xaxis(x=2:7,dir='d',tics='v') 

xaxis(x=[2,7,3],y=4,dir='d',tics='r') 
xaxis(x=4,y=2:7,dir='r',tics='v')
xaxis(x=2,y=[2,7,3],dir='l',tics='r')
xaxis(y=2:7,dir='r',tics='v')
xaxis(y=2:7,dir='l',tics='v')

xaxis(x=2:7,y=8,dir='u',tics='v',val='A'+string(1:6));
xaxis(x=8,y=2:7,dir='r',tics='v',val='B'+string(1:6));
 
xaxis(x=[2,5,0,3],y=9,dir='u',tics='i');
xaxis(x=9,y=[2,5,0,3],dir='r',tics='i',sub_int=5);

xaxis(x=2:7,y=4,dir='u',tics='v',fontsize=10,textcolor=9,ticscolor=7,seg=0,sub_int=20) // ok
