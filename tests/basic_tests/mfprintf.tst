// Copyright INRIA/ENPC

deff('[rep]=Fprintf(varargin)',['fd=mopen(TMPDIR+''/fprintf.rep'',''w'');';
	'varargin(0)=fd;mfprintf(varargin(:));mclose(fd);';
	'fd=mopen(TMPDIR+''/fprintf.rep'',''r'');';
	'str=mgetstr(100,fd);mclose(fd);rep=str;']);

deff('[y]=bugnum(str1,str2,str3)',['[lhs,rhs]=argn(0);if rhs==3 then y= str1<>str2 & str1<>str3;';
		'else y=str1<>str2;end'])

//----------test format '%f'

if bugnum(Fprintf('%f',-35),'-35.000000' ) then pause,end
if bugnum(Fprintf('%f',35.55),'35.550000' ) then pause,end
if bugnum(Fprintf('%f',0.00433),'0.004330' , ' .004330' ) then pause,end
if bugnum(Fprintf('%f',0.0000000345456),'0.000000' , ' .000000' ) then pause,end
if bugnum(Fprintf('%f',1112423453),'1112423453.000000' ) then pause,end

if bugnum(Fprintf('%15f',-35),'     -35.000000' ) then pause,end
if bugnum(Fprintf('%15f',0.00433),'       0.004330','        .004330' ) then pause,end
if bugnum(Fprintf('%15f',0.0000000345456),'       0.000000' ,'        .000000' ) then pause,end
if bugnum(Fprintf('%15f',1112423453),'1112423453.000000' ) then pause,end

if bugnum(Fprintf('%.1f',-35),'-35.0' ) then pause,end
if bugnum(Fprintf('%.0f',-35),'-35' ) then pause,end
if bugnum(Fprintf('%#.0f',-35),'-35.' ) then pause,end
if bugnum(Fprintf('%.1f',0.00433),'0.0',' .0') then pause,end
if bugnum(Fprintf('%.15f',0.0000000345456),'0.000000034545600' ,' .000000034545600' ) then pause,end
if bugnum(Fprintf('%.1f',11124234534545),'11124234534545.0' ) then pause,end

//----------test format '%g'
if bugnum(Fprintf('%g',-35),'-35' ) then pause,end
if bugnum(Fprintf('%g',35.55),'35.55' ) then pause,end
if bugnum(Fprintf('%g',35.551234567890),'35.5512' ) then pause,end
if bugnum(Fprintf('%+g',35.551234567890),'+35.5512' ) then pause,end
if bugnum(Fprintf('%g',0.00433),'0.00433' ,' .00433' ) then pause,end
if bugnum(Fprintf('%g',0.0000000345456),'3.45456e-08','3.45456e-008' ) then pause,end
if bugnum(Fprintf('%g',11124234534545),'1.11242e+13','1.11242e+013' ) then pause,end

if bugnum(Fprintf('%15g',-35),'            -35' ) then pause,end
if bugnum(Fprintf('%15g',0.00433),'        0.00433','         .00433' ) then pause,end
if bugnum(Fprintf('%15g',0.0000000345456),'    3.45456e-08','   3.45456e-008' ) then pause,end
if bugnum(Fprintf('%15g',11124234534545),'    1.11242e+13','   1.11242e+013' ) then pause,end

if bugnum(Fprintf('%.1g',-35.1),'-4e+01','-4e+001' ) then pause,end
if bugnum(Fprintf('%.0g',-35.1),'-4e+01','-4e+001'  ) then pause,end
if bugnum(Fprintf('%#.0g',-35.1),'-4.e+01','-4.e+001' ) then pause,end
if bugnum(Fprintf('%#.0G',-35.1),'-4.E+01','-4.E+001' ) then pause,end
if bugnum(Fprintf('%.1g',0.00433),'0.004' ,' .004' ) then pause,end
if bugnum(Fprintf('%.15g',0.0000000345456),'3.45456e-08','3.45456e-008' ) then pause,end
if bugnum(Fprintf('%.1g',11124234534545),'1e+13','1e+013' ) then pause,end

//----------test format '%e'
if bugnum(Fprintf('%e',-35),'-3.500000e+01','-3.500000e+001' ) then pause,end
if bugnum(Fprintf('%e',35.55),'3.555000e+01','3.555000e+001' ) then pause,end
if bugnum(Fprintf('%+e',35.55),'+3.555000e+01','+3.555000e+001' ) then pause,end
if bugnum(Fprintf('%e',35.551234567890),'3.555123e+01','3.555123e+001' ) then pause,end
if bugnum(Fprintf('%e',0.00433),'4.330000e-03','4.330000e-003' ) then pause,end
if bugnum(Fprintf('%e',0.0000000345456),'3.454560e-08','3.454560e-008' ) then pause,end
if bugnum(Fprintf('%e',11124234534545),'1.112423e+13','1.112423e+013' ) then pause,end
if bugnum(Fprintf('%E',11124234534545),'1.112423E+13','1.112423E+013' ) then pause,end

if bugnum(Fprintf('%15e',-35),'  -3.500000e+01',' -3.500000e+001' ) then pause,end
if bugnum(Fprintf('%15e',0.00433),'   4.330000e-03','  4.330000e-003' ) then pause,end
if bugnum(Fprintf('%15e',0.0000000345456),'   3.454560e-08','  3.454560e-008' ) then pause,end
if bugnum(Fprintf('%+15e',0.0000000345456),'  +3.454560e-08',' +3.454560e-008' ) then pause,end
if bugnum(Fprintf('%15e',11124234534545),'   1.112423e+13','  1.112423e+013' ) then pause,end

if bugnum(Fprintf('%.1e',-35),'-3.5e+01','-3.5e+001' ) then pause,end
if bugnum(Fprintf('%.0e',-35.1),'-4e+01' ,'-4e+001' ) then pause,end
if bugnum(Fprintf('%#.0e',-35.1),'-4.e+01','-4.e+001' ) then pause,end
if bugnum(Fprintf('%.1e',0.00433),'4.3e-03','4.3e-003' ) then pause,end
if bugnum(Fprintf('%.15e',0.0000000345456),'3.454560000000000e-08','3.454560000000000e-008' ) then pause,end
if bugnum(Fprintf('%.1e',11124234534545),'1.1e+13','1.1e+013' ) then pause,end

//----------test format %c
if bugnum(Fprintf('%c','t'),'t' ) then pause,end
if bugnum(Fprintf('%10c','t'),'         t' ) then pause,end
if bugnum(Fprintf('%10.3c','t'),'         t' ) then pause,end
if bugnum(Fprintf('%-10c','t'),'t         ' ) then pause,end

//----------test format %s
if bugnum(Fprintf('%s','text'),'text' ) then pause,end
if bugnum(Fprintf('%10s','text'),'      text' ) then pause,end
if bugnum(Fprintf('%10.3s','text'),'       tex' ) then pause,end
if bugnum(Fprintf('%-10s','text'),'text      ' ) then pause,end

if bugnum(Fprintf('%s','t'),'t' ) then pause,end

//----------test format %x
if bugnum(Fprintf('%x',123),'7b' ) then pause,end
if bugnum(Fprintf('%.10x',123),'000000007b' ) then pause,end
if bugnum(Fprintf('%x',-123),'ffffff85' ) then pause,end
if bugnum(Fprintf('%X',-123),'FFFFFF85' ) then pause,end
if bugnum(Fprintf('%#.3X',12),'0X00C' ) then pause,end

//----------test format %o
//if bugnum(Fprintf('%015o',-12),'000037777777764' ) then pause,end


