tutorial_path=get_file_path('libtutorial.tst');
exec(tutorial_path+'/loader.sce');

exec('ex01.sce')
exec('ex02.sce')
exec('ex03.sce')
if ~MSDOS then exec('ex04.sce'),end
if ~MSDOS then exec('ex05.sce'),end
exec('ex06.sce')
if ~MSDOS then exec('ex07.sce'),end
exec('ex08.sce')
exec('ex09.sce')
exec('ex10.sce')
if ~MSDOS then exec('ex11.sce'),end
exec('ex12.sce')
exec('ex13.sce')
exec('ex14.sce')
exec('ex15c.sce')
exec('ex15f.sce')
exec('ex16.sce')
exec('ex17.sce')
