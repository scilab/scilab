// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//test format '%f'
//
deff('[y]=bugnum(str1,str2,str3)',['[lhs,rhs]=argn(0);if rhs==3 then y= str1<>str2 & str1<>str3;';
		'else y=str1<>str2;end'])

if bugnum(sprintf('%f',-35),'-35.000000' ) then pause,end
if bugnum(sprintf('%f',35.55),'35.550000' ) then pause,end
if bugnum(sprintf('%f',0.00433),'0.004330' , ' .004330' ) then pause,end
if bugnum(sprintf('%f',0.0000000345456),'0.000000' , ' .000000' ) then pause,end
if bugnum(sprintf('%f',11124234534545),'11124234534545.000000' ) then pause,end

if bugnum(sprintf('%15f',-35),'     -35.000000' ) then pause,end
if bugnum(sprintf('%15f',0.00433),'       0.004330','        .004330' ) then pause,end
if bugnum(sprintf('%15f',0.0000000345456),'       0.000000' ,'        .000000' ) then pause,end
if bugnum(sprintf('%15f',11124234534545),'11124234534545.000000' ) then pause,end

if bugnum(sprintf('%.1f',-35),'-35.0' ) then pause,end
if bugnum(sprintf('%.0f',-35),'-35' ) then pause,end
if bugnum(sprintf('%#.0f',-35),'-35.' ) then pause,end
if bugnum(sprintf('%.1f',0.00433),'0.0',' .0') then pause,end
if bugnum(sprintf('%.15f',0.0000000345456),'0.000000034545600' ,' .000000034545600' ) then pause,end
if bugnum(sprintf('%.1f',11124234534545),'11124234534545.0' ) then pause,end

//test format '%g'
if bugnum(sprintf('%g',-35),'-35' ) then pause,end
if bugnum(sprintf('%g',35.55),'35.55' ) then pause,end
if bugnum(sprintf('%g',35.551234567890),'35.5512' ) then pause,end
if bugnum(sprintf('%+g',35.551234567890),'+35.5512' ) then pause,end
if bugnum(sprintf('%g',0.00433),'0.00433' ,' .00433' ) then pause,end
if bugnum(sprintf('%g',0.0000000345456),'3.45456e-08','3.45456e-008' ) then pause,end
if bugnum(sprintf('%g',11124234534545),'1.11242e+13','1.11242e+013' ) then pause,end

if bugnum(sprintf('%15g',-35),'            -35' ) then pause,end
if bugnum(sprintf('%15g',0.00433),'        0.00433','         .00433' ) then pause,end
if bugnum(sprintf('%15g',0.0000000345456),'    3.45456e-08','   3.45456e-008' ) then pause,end
if bugnum(sprintf('%15g',11124234534545),'    1.11242e+13','   1.11242e+013' ) then pause,end

if bugnum(sprintf('%.1g',-35.1),'-4e+01','-4e+001' ) then pause,end
if bugnum(sprintf('%.0g',-35.1),'-4e+01','-4e+001' ) then pause,end
if bugnum(sprintf('%#.0g',-35.1),'-4.e+01','-4.e+001' ) then pause,end
if bugnum(sprintf('%#.0G',-35.1),'-4.E+01','-4.E+001' ) then pause,end
if bugnum(sprintf('%.1g',0.00433),'0.004' ,' .004' ) then pause,end
if bugnum(sprintf('%.15g',0.0000000345456),'3.45456e-08','3.45456e-008' ) then pause,end
if bugnum(sprintf('%.1g',11124234534545),'1e+13','1e+013' ) then pause,end

//test format '%e'
if bugnum(sprintf('%e',-35),'-3.500000e+01' ,'-3.500000e+001') then pause,end
if bugnum(sprintf('%e',35.55),'3.555000e+01','3.555000e+001' ) then pause,end
if bugnum(sprintf('%+e',35.55),'+3.555000e+01','+3.555000e+001' ) then pause,end
if bugnum(sprintf('%e',35.551234567890),'3.555123e+01' ,'3.555123e+001' ) then pause,end
if bugnum(sprintf('%e',0.00433),'4.330000e-03','4.330000e-003' ) then pause,end
if bugnum(sprintf('%e',0.0000000345456),'3.454560e-08','3.454560e-008' ) then pause,end
if bugnum(sprintf('%e',11124234534545),'1.112423e+13','1.112423e+013' ) then pause,end
if bugnum(sprintf('%E',11124234534545),'1.112423E+13','1.112423E+013' ) then pause,end

if bugnum(sprintf('%15e',-35),'  -3.500000e+01',' -3.500000e+001' ) then pause,end
if bugnum(sprintf('%15e',0.00433),'   4.330000e-03','  4.330000e-003' ) then pause,end
if bugnum(sprintf('%15e',0.0000000345456),'   3.454560e-08','  3.454560e-008' ) then pause,end
if bugnum(sprintf('%+15e',0.0000000345456),'  +3.454560e-08',' +3.454560e-008') then pause,end
if bugnum(sprintf('%15e',11124234534545),'   1.112423e+13','  1.112423e+013' ) then pause,end

if bugnum(sprintf('%.1e',-35),'-3.5e+01','-3.5e+001' ) then pause,end
if bugnum(sprintf('%.0e',-35.1),'-4e+01','-4e+001' ) then pause,end
if bugnum(sprintf('%#.0e',-35.1),'-4.e+01','-4.e+001' ) then pause,end
if bugnum(sprintf('%.1e',0.00433),'4.3e-03','4.3e-003' ) then pause,end
if bugnum(sprintf('%.15e',0.0000000345456),'3.454560000000000e-08','3.454560000000000e-008' ) then pause,end
if bugnum(sprintf('%.1e',11124234534545),'1.1e+13','1.1e+013' ) then pause,end

//test format %c
//if bugnum(sprintf('%c',29),'t' ) then pause,end
//if bugnum(sprintf('%10c',-29),'         T' ) then pause,end
//if bugnum(sprintf('%10.3c',29),'         t' ) then pause,end
//if bugnum(sprintf('%-10c',29),'t         ' ) then pause,end

//test format %s
if bugnum(sprintf('%s','text'),'text' ) then pause,end
if bugnum(sprintf('%10s','text'),'      text' ) then pause,end
if bugnum(sprintf('%10.3s','text'),'       tex' ) then pause,end
if bugnum(sprintf('%-10s','text'),'text      ' ) then pause,end

if bugnum(sprintf('%s','t'),'t' ) then pause,end

//test format %x
if bugnum(sprintf('%x',123),'7b' ) then pause,end
if bugnum(sprintf('%.10x',123),'000000007b' ) then pause,end
if bugnum(sprintf('%x',-123),'ffffff85' ) then pause,end
if bugnum(sprintf('%X',-123),'FFFFFF85' ) then pause,end
if bugnum(sprintf('%#.3X',12),'0X00C' ) then pause,end

//test format %o
//if bugnum(sprintf('%015o',-12),'000037777777764' ) then pause,end

