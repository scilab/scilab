How to compile with visual studio C++ .Net 2003 and Intel Fortran 8.0 ?

Pre-requirements:

Install Visual Studio C++ .NET 2003
Install Intel Fortran 8.0
Install TCL/TK 8.3


1] copy tcl83.dll and tk83.dll in bin directory.
2] edit make-pvm3.mak in pvm3 directory
   and modify first line (PVM_ROOT=...) with the good path
3] execute configheaders.bat
4] copy in root of scilab directory : "lib" and "tcllib" directories (see below)
5] click on scilab.sln (to launch Visual) or devenv scilab.sln /build release (via command line, to build release)


N.B:
if you have this message : "fatal error LNK1127: library is corrupt"
disable your antivirus and rebuild.

contents of the "lib" directory:

 Répertoire de D:\scilab\lib

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
09/02/2004  15:20    <REP>          TCL8.3
09/02/2004  15:20    <REP>          TK8.3
               0 fichier(s)                0 octets

 Répertoire de D:\scilab\lib\TCL8.3

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08            19 170 AUTO.TCL
09/02/2004  15:20    <REP>          ENCODING
31/01/2003  09:08             9 007 HISTORY.TCL
09/02/2004  15:20    <REP>          HTTP1.0
09/02/2004  15:20    <REP>          HTTP2.4
31/01/2003  09:08            18 717 INIT.TCL
31/01/2003  09:08             6 798 ldAout.tcl
09/02/2004  15:20    <REP>          MSGCAT1.1
09/02/2004  15:20    <REP>          OPT0.4
31/01/2003  09:08            20 576 PACKAGE.TCL
31/01/2003  09:08               882 PARRAY.TCL
31/01/2003  09:08            27 596 SAFE.TCL
31/01/2003  09:08             6 009 tclIndex
09/02/2004  15:20    <REP>          TCLTEST1.0
31/01/2003  09:08             4 363 WORD.TCL
               9 fichier(s)          113 118 octets

 Répertoire de D:\scilab\lib\TCL8.3\ENCODING

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08             1 090 ASCII.ENC
31/01/2003  09:08            92 873 BIG5.ENC
31/01/2003  09:08             1 091 CP1250.ENC
31/01/2003  09:08             1 091 CP1251.ENC
31/01/2003  09:08             1 091 CP1252.ENC
31/01/2003  09:08             1 091 CP1253.ENC
31/01/2003  09:08             1 091 CP1254.ENC
31/01/2003  09:08             1 091 CP1255.ENC
31/01/2003  09:08             1 091 CP1256.ENC
31/01/2003  09:08             1 091 CP1257.ENC
31/01/2003  09:08             1 091 CP1258.ENC
31/01/2003  09:08             1 090 CP437.ENC
31/01/2003  09:08             1 090 CP737.ENC
31/01/2003  09:08             1 090 CP775.ENC
31/01/2003  09:08             1 090 CP850.ENC
31/01/2003  09:08             1 090 CP852.ENC
31/01/2003  09:08             1 090 CP855.ENC
31/01/2003  09:08             1 090 CP857.ENC
31/01/2003  09:08             1 090 CP860.ENC
31/01/2003  09:08             1 090 CP861.ENC
31/01/2003  09:08             1 090 CP862.ENC
31/01/2003  09:08             1 090 CP863.ENC
31/01/2003  09:08             1 090 CP864.ENC
31/01/2003  09:08             1 090 CP865.ENC
31/01/2003  09:08             1 090 CP866.ENC
31/01/2003  09:08             1 090 CP869.ENC
31/01/2003  09:08             1 090 CP874.ENC
31/01/2003  09:08            48 025 CP932.ENC
31/01/2003  09:08           132 509 CP936.ENC
31/01/2003  09:08           130 423 CP949.ENC
31/01/2003  09:08            91 831 CP950.ENC
31/01/2003  09:08             1 093 DINGBATS.ENC
31/01/2003  09:08             1 054 EBCDIC.ENC
31/01/2003  09:08            85 574 EUC-CN.ENC
31/01/2003  09:08            82 445 EUC-JP.ENC
31/01/2003  09:08            93 918 EUC-KR.ENC
31/01/2003  09:08            86 619 GB12345.ENC
31/01/2003  09:08             1 091 GB1988.ENC
31/01/2003  09:08            84 532 GB2312.ENC
31/01/2003  09:08               195 iso2022-jp.enc
31/01/2003  09:08               115 iso2022-kr.enc
31/01/2003  09:08               226 ISO2022.ENC
31/01/2003  09:08             1 094 iso8859-1.enc
31/01/2003  09:08             1 095 iso8859-10.enc
31/01/2003  09:08             1 095 iso8859-13.enc
31/01/2003  09:08             1 095 iso8859-14.enc
31/01/2003  09:08             1 095 iso8859-15.enc
31/01/2003  09:08             1 095 iso8859-16.enc
31/01/2003  09:08             1 094 iso8859-2.enc
31/01/2003  09:08             1 094 iso8859-3.enc
31/01/2003  09:08             1 094 iso8859-4.enc
31/01/2003  09:08             1 094 iso8859-5.enc
31/01/2003  09:08             1 094 iso8859-6.enc
31/01/2003  09:08             1 094 iso8859-7.enc
31/01/2003  09:08             1 094 iso8859-8.enc
31/01/2003  09:08             1 094 iso8859-9.enc
31/01/2003  09:08             1 092 JIS0201.ENC
31/01/2003  09:08            80 361 JIS0208.ENC
31/01/2003  09:08            70 974 JIS0212.ENC
31/01/2003  09:08             1 091 KOI8-R.ENC
31/01/2003  09:08             1 300 KOI8-U.ENC
31/01/2003  09:08            92 877 KSC5601.ENC
31/01/2003  09:08             1 096 macCentEuro.enc
31/01/2003  09:08             1 096 macCroatian.enc
31/01/2003  09:08             1 096 macCyrillic.enc
31/01/2003  09:08             1 096 macDingbats.enc
31/01/2003  09:08             1 093 macGreek.enc
31/01/2003  09:08             1 095 macIceland.enc
31/01/2003  09:08            48 028 macJapan.enc
31/01/2003  09:08             1 093 macRoman.enc
31/01/2003  09:08             1 095 macRomania.enc
31/01/2003  09:08             1 092 macThai.enc
31/01/2003  09:08             1 095 macTurkish.enc
31/01/2003  09:08             1 095 macUkraine.enc
31/01/2003  09:08            41 770 SHIFTJIS.ENC
31/01/2003  09:08             1 091 SYMBOL.ENC
31/01/2003  09:08             1 091 TIS-620.ENC
              77 fichier(s)        1 327 910 octets

 Répertoire de D:\scilab\lib\TCL8.3\HTTP1.0

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08             9 759 HTTP.TCL
31/01/2003  09:08               735 pkgIndex.tcl
               2 fichier(s)           10 494 octets

 Répertoire de D:\scilab\lib\TCL8.3\HTTP2.4

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08            23 473 HTTP.TCL
31/01/2003  09:08               726 pkgIndex.tcl
               2 fichier(s)           24 199 octets

 Répertoire de D:\scilab\lib\TCL8.3\MSGCAT1.1

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08             5 189 MSGCAT.TCL
31/01/2003  09:08               134 pkgIndex.tcl
               2 fichier(s)            5 323 octets

 Répertoire de D:\scilab\lib\TCL8.3\OPT0.4

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08            33 266 OPTPARSE.TCL
31/01/2003  09:08               605 pkgIndex.tcl
               2 fichier(s)           33 871 octets

 Répertoire de D:\scilab\lib\TCL8.3\TCLTEST1.0

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08               602 pkgIndex.tcl
31/01/2003  09:08            59 190 TCLTEST.TCL
               2 fichier(s)           59 792 octets

 Répertoire de D:\scilab\lib\TK8.3

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08             8 598 BGERROR.TCL
31/01/2003  09:08            11 519 BUTTON.TCL
31/01/2003  09:08             8 036 choosedir.tcl
31/01/2003  09:08            19 618 CLRPICK.TCL
31/01/2003  09:08             7 426 COMDLG.TCL
31/01/2003  09:08            26 364 CONSOLE.TCL
09/02/2004  15:20    <REP>          DEMOS
31/01/2003  09:08             6 218 DIALOG.TCL
31/01/2003  09:08            15 939 ENTRY.TCL
31/01/2003  09:08             5 052 FOCUS.TCL
09/02/2004  15:20    <REP>          IMAGES
31/01/2003  09:08            13 507 LISTBOX.TCL
31/01/2003  09:08            36 764 MENU.TCL
31/01/2003  09:08            15 301 MSGBOX.TCL
31/01/2003  09:08               803 OBSOLETE.TCL
31/01/2003  09:08             1 659 optMenu.tcl
31/01/2003  09:08             7 396 PALETTE.TCL
31/01/2003  09:08               437 pkgIndex.tcl
31/01/2003  09:08             9 568 PROLOG.PS
31/01/2003  09:08             7 544 SAFETK.TCL
31/01/2003  09:08             7 019 SCALE.TCL
31/01/2003  09:08            11 743 SCRLBAR.TCL
31/01/2003  09:08            18 465 tclIndex
31/01/2003  09:08             4 657 TEAROFF.TCL
31/01/2003  09:08            28 376 TEXT.TCL
31/01/2003  09:08            11 299 TK.TCL
31/01/2003  09:08             4 273 tkAppInit.c
31/01/2003  09:08            40 747 TKFBOX.TCL
31/01/2003  09:08            21 835 XMFBOX.TCL
              27 fichier(s)          350 163 octets

 Répertoire de D:\scilab\lib\TK8.3\DEMOS

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08             8 015 ARROW.TCL
31/01/2003  09:08             3 086 BIND.TCL
31/01/2003  09:08             1 627 BITMAP.TCL
31/01/2003  09:08             1 814 BROWSE
31/01/2003  09:08             1 484 BUTTON.TCL
31/01/2003  09:08             1 369 CHECK.TCL
31/01/2003  09:08             1 628 CLRPICK.TCL
31/01/2003  09:08             5 193 COLORS.TCL
31/01/2003  09:08             3 180 CSCROLL.TCL
31/01/2003  09:08             4 987 CTEXT.TCL
31/01/2003  09:08               731 DIALOG1.TCL
31/01/2003  09:08               683 DIALOG2.TCL
31/01/2003  09:08             1 583 ENTRY1.TCL
31/01/2003  09:08             2 309 ENTRY2.TCL
31/01/2003  09:08             2 133 FILEBOX.TCL
31/01/2003  09:08            79 361 FLOOR.TCL
31/01/2003  09:08             1 242 FORM.TCL
31/01/2003  09:08               552 HELLO
31/01/2003  09:08             1 693 HSCALE.TCL
31/01/2003  09:08             2 206 ICON.TCL
31/01/2003  09:08             1 148 IMAGE1.TCL
31/01/2003  09:08             2 625 IMAGE2.TCL
09/02/2004  15:20    <REP>          IMAGES
31/01/2003  09:08             9 864 ITEMS.TCL
31/01/2003  09:08             7 570 IXSET
31/01/2003  09:08             1 462 LABEL.TCL
31/01/2003  09:08             6 414 MENU.TCL
31/01/2003  09:08             4 481 MENUBU.TCL
31/01/2003  09:08             2 175 MSGBOX.TCL
31/01/2003  09:08             2 933 PLOT.TCL
31/01/2003  09:08             2 522 PUZZLE.TCL
31/01/2003  09:08             1 700 RADIO.TCL
31/01/2003  09:08             2 135 README
31/01/2003  09:08             5 315 RMT
31/01/2003  09:08             8 251 ROLODEX
31/01/2003  09:08             5 360 RULER.TCL
31/01/2003  09:08             2 397 SAYINGS.TCL
31/01/2003  09:08             4 596 SEARCH.TCL
31/01/2003  09:08             1 268 SQUARE
31/01/2003  09:08             1 865 STATES.TCL
31/01/2003  09:08             6 962 STYLE.TCL
31/01/2003  09:08             4 354 tclIndex
31/01/2003  09:08            11 511 TCOLOR
31/01/2003  09:08             3 360 TEXT.TCL
31/01/2003  09:08             1 132 TIMER
31/01/2003  09:08             6 735 TWIND.TCL
31/01/2003  09:08             1 673 VSCALE.TCL
31/01/2003  09:08            13 723 WIDGET.TCL
              47 fichier(s)          248 407 octets

 Répertoire de D:\scilab\lib\TK8.3\DEMOS\IMAGES

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08            51 712 EARTH.GIF
31/01/2003  09:08             6 343 EARTHRIS.GIF
31/01/2003  09:08            12 720 FACE.BMP
31/01/2003  09:08             1 886 FLAGDOWN.BMP
31/01/2003  09:08             1 880 FLAGUP.BMP
31/01/2003  09:08               275 GRAY25.BMP
31/01/2003  09:08             1 883 LETTERS.BMP
31/01/2003  09:08             1 889 NOLETTER.BMP
31/01/2003  09:08               272 PATTERN.BMP
31/01/2003  09:08             2 341 TCLLOGO.GIF
31/01/2003  09:08           196 623 TEAPOT.PPM
              11 fichier(s)          277 824 octets

 Répertoire de D:\scilab\lib\TK8.3\IMAGES

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
31/01/2003  09:08            11 006 ActiveTclSplash.gif
31/01/2003  09:08            32 900 LOGO.EPS
31/01/2003  09:08             2 341 LOGO100.GIF
31/01/2003  09:08             1 670 LOGO64.GIF
31/01/2003  09:08            11 000 logoLarge.gif
31/01/2003  09:08             3 889 logoMed.gif
31/01/2003  09:08            27 809 pwrdLogo.eps
31/01/2003  09:08             1 615 pwrdLogo100.gif
31/01/2003  09:08             2 489 pwrdLogo150.gif
31/01/2003  09:08             2 981 pwrdLogo175.gif
31/01/2003  09:08             3 491 pwrdLogo200.gif
31/01/2003  09:08             1 171 pwrdLogo75.gif
31/01/2003  09:08               388 README
31/01/2003  09:08             5 473 TAI-KU.GIF
              14 fichier(s)          108 223 octets

     Total des fichiers listés :
             195 fichier(s)        2 559 324 octets
              35 Rép(s)  12 611 510 272 octets libres


And contents of the TCLLIB directory:

 Répertoire de D:\scilab\TCLLIB

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
09/02/2004  15:20    <REP>          INCLUDE
09/02/2004  15:20    <REP>          LIB
               0 fichier(s)                0 octets

 Répertoire de D:\scilab\TCLLIB\INCLUDE

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
03/08/2003  19:20             6 454 ITCL.H
03/08/2003  19:20             8 815 itclDecls.h
03/08/2003  19:20            11 159 itclInt.h
03/08/2003  19:20            41 825 itclIntDecls.h
03/08/2003  19:20             3 797 ITK.H
03/08/2003  19:20             6 771 itkDecls.h
03/08/2003  19:20             1 256 JCONFIG.H
03/08/2003  19:20             1 221 proTbcLoad.h
03/08/2003  19:20            57 863 TCL.H
03/08/2003  19:20           129 917 tclDecls.h
03/08/2003  19:20             9 991 tclExtend.h
03/08/2003  19:20            91 810 tclInt.h
03/08/2003  19:20            53 118 tclIntDecls.h
03/08/2003  19:20            19 022 tclIntPlatDecls.h
03/08/2003  19:20             4 806 tclPlatDecls.h
03/08/2003  19:20            47 404 TK.H
03/08/2003  19:20            80 495 tkDecls.h
03/08/2003  19:20            63 562 tkIntXlibDecls.h
03/08/2003  19:20             6 508 tkPlatDecls.h
03/08/2003  19:20            24 889 tkTable.h
09/02/2004  15:20    <REP>          X11
03/08/2003  19:20            20 829 XMLPARSE.H
03/08/2003  19:20             3 836 XMLROLE.H
03/08/2003  19:20            11 189 XMLTOK.H
03/08/2003  19:20             7 870 ZCONF.H
03/08/2003  19:20            40 898 ZLIB.H
              25 fichier(s)          755 305 octets

 Répertoire de D:\scilab\TCLLIB\INCLUDE\X11

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
03/08/2003  19:20             1 953 cursorfont.h
03/08/2003  19:20             1 501 KEYSYM.H
03/08/2003  19:20            52 429 keysymdef.h
03/08/2003  19:20            18 639 X.H
03/08/2003  19:20             2 518 Xatom.h
03/08/2003  19:20             1 720 Xfuncproto.h
03/08/2003  19:20            40 708 Xlib.h
03/08/2003  19:20            20 215 Xutil.h
               8 fichier(s)          139 683 octets

 Répertoire de D:\scilab\TCLLIB\LIB

09/02/2004  15:20    <REP>          .
09/02/2004  15:20    <REP>          ..
03/08/2003  19:20           508 562 TCL83.LIB
03/08/2003  19:20           495 056 TK83.LIB
               2 fichier(s)        1 003 618 octets

     Total des fichiers listés :
              35 fichier(s)        1 898 606 octets
              11 Rép(s)  12 611 510 272 octets libres