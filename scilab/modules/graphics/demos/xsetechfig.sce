mode(-1)
xbasc() ;

xset('color',2)

wrect=[0.2 0.1 0.6 0.6] ;
frect=[0 -1 10 1] ;// coordonnees utilsateurs
dx=frect(3)-frect(1) ;ex=0.07*dx
dy=frect(4)-frect(2) ;ey=0.07*dy
f=1/7 ;arect=ones(4,1)*f

xsetech(wrect=wrect,frect=frect,arect=arect)
xset('thickness',3)
L=dx/(1-2*f)
H=dy/(1-2*f)
Lt=L/wrect(3)
Ht=H/wrect(4)
//Full window rectangle
//=====================
xrect(frect(1)-f*L-wrect(1)*Lt+0.03,frect(4)+f*H+wrect(2)*Ht-0.01,Lt-0.06,Ht-0.04)
xstring(frect(1)-f*L-wrect(1)*Lt+ex/2,frect(4)+f*H+wrect(2)*Ht-Ht+ey,'(0,0)')
xstring(frect(1)-f*L-wrect(1)*Lt+ex/2,frect(4)+f*H+wrect(2)*Ht-2*ey,'(0,1)')
xstring(frect(1)-f*L-wrect(1)*Lt+Lt-2*ex,frect(4)+f*H+wrect(2)*Ht-Ht+ey,'(1,0)')
//Draw wrect relatives
//===================

xset('thickness',1) ;xset('color',2)
//origin
xsegs([frect(1)-f*L-wrect(1)*Lt ;frect(1)-f*L],..
    [frect(2)-f*H ;frect(2)-f*H]) ;
xstring(frect(1)-f*L-wrect(1)*Lt+ex,frect(2)-f*H,'xleft')

xsegs([frect(1)-f*L;frect(1)-f*L],..
    [frect(2)-f*H+H ;frect(4)+f*H+wrect(2)*Ht]) ;
xstring(frect(1)-f*L+ex/2,(frect(2)-f*H+H+frect(4)+f*H+wrect(2)*Ht)/2,'yup')
 
//width and height

;xset('font',4,2)
xsegs((frect(1)-f*L-ex)*ones(2,2),[frect(4)+f*H ,frect(4)+f*H-H/2-ey ;
                                   frect(4)+f*H-H/2+ey,frect(4)+f*H-H])
xstring(frect(1)-f*L-ex,frect(4)+f*H-H/2-ey/2,'h')

xsegs([frect(1)-f*L,frect(1)-f*L+L/2+ex
       frect(1)-f*L+L/2-ex,frect(1)-f*L+L],(frect(4)+f*H+ey)*ones(2,2))

xstring(frect(1)-f*L+L/2,frect(4)+f*H+ey/2,'w')
xset('font',2,1)


// wrect rectangle
xset('color',5)
xset('thickness',2)
xrect(frect(1)-f*L,frect(4)+f*H,L,H)
xset('thickness',1)
// width and height
xset('font',4,2) ;


xset('foreground',5)
drawaxis(x=frect(1)-f*L,y=frect(4)+f*H-H+[0,H],val=['0','1'],seg=0,sub_int=3)
drawaxis(x=frect(1)-f*L+[0,L],y=frect(4)+f*H-H,val=['0','1'],dir='d',seg=0,sub_int=3)
//xset('font',2,1)
//user coordinate rectangle
//=========================
xset('color',5) ;xset('thickness',1)
xsegs([4 ;4],[1 ;1+f*H]) ;xstring(4+ex/2,1+f*H/2-ey/2,'y_up')
xsegs([4 ;4],[-1 ;-1-f*H]);xstring(4+ex,-1-f*H/2,'y_down')
xsegs([10 ;10+f*L],[0 ;0]);xstring(10+ex/2,0,'x_right')
xsegs([0 ;0-f*L],[0 ;0]);xstring(ex/2-f*L,0,'x_left')
xset('color',0),xset('thickness',3)

//draw a curve inside user coordinate rectangle
//=============================================
xset('color',14) ;xset('foreground',14)

xrect(0,1,10,2)

xset('color',16)
r=xstringl(0,0,'yminx')
xstring(0-r(3),-1-r(2)/2,'ymin')
xstring(0-r(3),1-r(2)/2,'ymax')
r=xstringl(0,0,'xminx')
xstring(0-r(3)/2,-1-r(2)*1.5,'xmin')
xstring(10-r(3)/2,-1-r(2)*1.5,'xmax')
t=0 :0.1 :10 ;
plot2d(t,sin(t),style=16,axesflag=2)
xset('foreground',0) ;xset('color',0)

//Draw legend
txt=['wrect=[xleft,yup,w,h]'
     'arect=[x_left, x_right,y_up,y_down]'
     'frect=[xmin,ymin,xmax,ymax] //user co-ordinates'
     ' '
     'xsetech(wrect=wrect,frect=frect,arect=arect)']

xset('font',2,3)
r=xstringl(0,0,txt) 
y=frect(4)+f*H+wrect(2)*Ht-Ht+ey
x=frect(1)-f*L-wrect(1)*Lt+(Lt-r(3))/2


xstring(x,y,txt(5)) //xsetech
y=y+r(4)/5
xstring(x,y,txt(4)) //

y=y+r(4)/5
xset('color',16)
xstring(x,y,txt(3))//frect

y=y+r(4)/5
xset('color',14) //arect
k=strindex(txt(2),'=')
xstring(x,y,part(txt(2),1:k))
r1=xstringl(0,0,part(txt(2),1:k))
xset('color',5)
xstring(x+r1(3),y,part(txt(2),k+1:length(txt(2))))


y=y+r(4)/5 

k=strindex(txt(1),'=') //wrect
xset('color',5)
xstring(x,y,part(txt(1),1:k))
xset('color',2)
r1=xstringl(0,0,part(txt(1),1:k))
xstring(x+r1(3),y,part(txt(1),k+1:length(txt(1))))


xset('color',0)
