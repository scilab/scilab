// Copyright INRIA
// Author Jean-Baptiste Silvy

mode(-1);
clear ;

// test with one axes
axes1 = gca() ;
plot2d ;
cmpd1 = gce() ;
plot(0:10) ;
cmpd2 = gce() ;
relocate_handle( cmpd1.children(1), cmpd2 ) ;

// there should be two handles in each axes.
if ( size(cmpd1.children,'*' ) <> 2 | size(cmpd2.children,'*' ) <> 2 ) then pause ; end ;


// test with two axes
xdel(winsid()) ;

axes1 = gca() ;
subplot(211);
plot2d ;
cmpd1 = gce() ;

subplot(212);
plot(0:10) ;
cmpd2 = gce() ;
relocate_handle( cmpd1.children(1), cmpd2 ) ;

// there should be two handles in each axes.
if ( size(cmpd1.children,'*' ) <> 2 | size(cmpd2.children,'*' ) <> 2 ) then pause ; end ;

// test with two figures
xdel(winsid()) ;

axes1 = gca() ;
plot2d ;
cmpd1 = gce() ;

scf();
plot(0:10) ;
cmpd2 = gce() ;
relocate_handle( cmpd1.children(1), cmpd2 ) ;

// there should be two handles in each axes.
if ( size(cmpd1.children,'*' ) <> 2 | size(cmpd2.children,'*' ) <> 2 ) then pause ; end ;

// test relocating axes
xdel(winsid()) ;

fig1 = gcf()  ;
axes1 = gca() ;
plot2d ;

fig2  = scf() ;
plot2d ;

relocate_handle( axes1 , fig2 ) ;

if ( size( fig1.children,'*' ) <> 1 | size( fig2.children,'*' ) <> 2 ) then pause ; end ;
