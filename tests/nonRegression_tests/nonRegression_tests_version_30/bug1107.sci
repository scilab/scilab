// bug1107
//the bug was due to a misinterpretation of syntax like (x=2) in
//factor. It was treated as a named argument of a function instead of a
//factor equivalent to (x==2)
mode (-1)
clear
x=5; if (x = 4) then x=6;end
ok=x==5
ok=ok&execstr('x=5;z=2*(x=4)',"errcatch","n")==0&z==0
ok=ok&execstr('x=5;z=2*(x-1=4)',"errcatch","n")==0&z==2
affich_result(ok, 1107)
clear
