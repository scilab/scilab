function addmenus(nam,sub)
DATA_BEGIN=ascii(2);
DATA_END=ascii(5);
if argn(2)==1 then
scm='(set! texmacs-menu (menu-merge texmacs-menu   ''(=> ""'+nam+..
'"" (extern-exec ""execstr('+nam+')""))))'
write(%io(2),DATA_BEGIN+'command:'+scm+DATA_END,'(a)')
return
end

t=nam+'('+string(1:size(sub,'*'))'+')'
scm='(define '+nam+'-menu ''('+strcat('(""'+sub(:)+'"" (extern-exec ""execstr('+t+')""))',' ')+'))'
write(%io(2),DATA_BEGIN+'command:'+scm+DATA_END,'(a)')
scm='(set! texmacs-menu (menu-merge texmacs-menu   ''(=> ""'+nam+..
'"" (link '+nam+'-menu))))'
write(%io(2),DATA_BEGIN+'command:'+scm+DATA_END,'(a)')
endfunction
