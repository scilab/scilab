function ChangeAngledTextFont(h)
//Turpitude en attendant que l'on puisse tourner le text
//filled (scilab5) voir rotate_compound.sci case text

  if argn(2)==0 then h=gcf(),end
  select h.type
  case "Figure"   then
    n=size(h.children,'*');
    for k=1:n,   ChangeAngledTextFont(h.children(k));end
  case "Axes"   then
    n=size(h.children,'*');
    for k=1:n,   ChangeAngledTextFont(h.children(k));end
  case "Compound"   then
    n=size(h.children,'*');
    if type(h.user_data)==15&h.user_data(1)=="xstringb2" then
      //extract the text 
      txt=[]
      for k=1:n,txt=[txt;h.children(k).text],end
     //create a new objects with the good font size
     box=h.user_data(2);
     orig=h.user_data(3);
     RotCenter=h.user_data(4);
     theta=h.user_data(5);
      xstringb2(orig(1),orig(2),txt,box(1),box(2),'fill')
      e=gce();if e.type<>"Compound" then e=glue(e),end
      //apply font size and origin to h
       for k=1:n, 
	 rotate_text(h.children(k),theta,RotCenter),
	 h.children(k).font_size=e.children(k).font_size
	 h.children(k).data = e.children(k).data 
	 h.children(k).text_box= e.children(k).text_box
       end
       delete(e)
    else
      for i=1:size(h.children,'*')
	ChangeAngledTextFont(h.children(i));
      end
    end

  end
endfunction
