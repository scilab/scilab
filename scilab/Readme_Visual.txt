How to compile with visual studio C++ .Net 2003 and Intel Fortran 8.0 ?

Pre-requirements:

Install Visual Studio C++ .NET 2003
Install Intel Fortran 8.0
Install TCL/TK 8.4(.5)


1] copy tcl84.dll,tk84.dll,tcl84.lib and tk84.lib in bin directory.
2] edit Make-PVM3.mak in pvm3 directory
   and modify first line (PVM_ROOT=...) with the good path
3] execute configheaders.bat
4] copy in root of scilab directory : "lib" and "tcllib" directories (see below)
5] click on scilab.sln (to launch Visual) or devenv scilab.sln /build release (via command line, to build release)


N.B:
if you have this message : "fatal error LNK1127: library is corrupt"
disable your antivirus and rebuild.

contents of the "lib" directory:

Répertoire de d:\scilab\lib

13/02/2004  09:41    <REP>          .
13/02/2004  09:41    <REP>          ..
13/02/2004  09:40    <REP>          tcl8.4
13/02/2004  09:40    <REP>          tk8.4
               0 fichier(s)                0 octets

 Répertoire de d:\scilab\lib\tcl8.4

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            19 529 auto.tcl
13/02/2004  09:40    <REP>          encoding
24/11/2003  21:47             9 108 history.tcl
13/02/2004  09:40    <REP>          http1.0
13/02/2004  09:40    <REP>          http2.4
24/11/2003  21:48            23 431 init.tcl
24/11/2003  21:47             6 802 ldAout.tcl
13/02/2004  09:40    <REP>          msgcat1.3
13/02/2004  09:40    <REP>          opt0.4
24/11/2003  21:47            24 008 package.tcl
24/11/2003  21:47               882 parray.tcl
24/11/2003  21:47            27 614 safe.tcl
24/11/2003  21:47             6 097 tclIndex
13/02/2004  09:40    <REP>          tcltest2.2
24/11/2003  21:47             4 375 word.tcl
               9 fichier(s)          121 846 octets

 Répertoire de d:\scilab\lib\tcl8.4\encoding

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47             1 090 ascii.enc
24/11/2003  21:47            92 873 big5.enc
24/11/2003  21:47             1 091 cp1250.enc
24/11/2003  21:47             1 091 cp1251.enc
24/11/2003  21:47             1 091 cp1252.enc
24/11/2003  21:47             1 091 cp1253.enc
24/11/2003  21:47             1 091 cp1254.enc
24/11/2003  21:47             1 091 cp1255.enc
24/11/2003  21:47             1 091 cp1256.enc
24/11/2003  21:47             1 091 cp1257.enc
24/11/2003  21:47             1 091 cp1258.enc
24/11/2003  21:47             1 090 cp437.enc
24/11/2003  21:47             1 090 cp737.enc
24/11/2003  21:47             1 090 cp775.enc
24/11/2003  21:47             1 090 cp850.enc
24/11/2003  21:47             1 090 cp852.enc
24/11/2003  21:47             1 090 cp855.enc
24/11/2003  21:47             1 090 cp857.enc
24/11/2003  21:47             1 090 cp860.enc
24/11/2003  21:47             1 090 cp861.enc
24/11/2003  21:47             1 090 cp862.enc
24/11/2003  21:47             1 090 cp863.enc
24/11/2003  21:47             1 090 cp864.enc
24/11/2003  21:47             1 090 cp865.enc
24/11/2003  21:47             1 090 cp866.enc
24/11/2003  21:47             1 090 cp869.enc
24/11/2003  21:47             1 090 cp874.enc
24/11/2003  21:47            48 207 cp932.enc
24/11/2003  21:47           132 509 cp936.enc
24/11/2003  21:47           130 423 cp949.enc
24/11/2003  21:47            91 831 cp950.enc
24/11/2003  21:47             1 093 dingbats.enc
24/11/2003  21:47             1 054 ebcdic.enc
24/11/2003  21:47            85 574 euc-cn.enc
24/11/2003  21:47            82 537 euc-jp.enc
24/11/2003  21:47            93 918 euc-kr.enc
24/11/2003  21:47            86 619 gb12345.enc
24/11/2003  21:47             1 091 gb1988.enc
24/11/2003  21:47            84 532 gb2312-raw.enc
24/11/2003  21:47            85 574 gb2312.enc
24/11/2003  21:47               192 iso2022-jp.enc
24/11/2003  21:47               115 iso2022-kr.enc
24/11/2003  21:47               226 iso2022.enc
24/11/2003  21:47             1 094 iso8859-1.enc
24/11/2003  21:47             1 095 iso8859-10.enc
24/11/2003  21:47             1 095 iso8859-13.enc
24/11/2003  21:47             1 095 iso8859-14.enc
24/11/2003  21:47             1 095 iso8859-15.enc
24/11/2003  21:47             1 095 iso8859-16.enc
24/11/2003  21:47             1 094 iso8859-2.enc
24/11/2003  21:47             1 094 iso8859-3.enc
24/11/2003  21:47             1 094 iso8859-4.enc
24/11/2003  21:47             1 094 iso8859-5.enc
24/11/2003  21:47             1 094 iso8859-6.enc
24/11/2003  21:47             1 094 iso8859-7.enc
24/11/2003  21:47             1 094 iso8859-8.enc
24/11/2003  21:47             1 094 iso8859-9.enc
24/11/2003  21:47             1 092 jis0201.enc
24/11/2003  21:47            80 459 jis0208.enc
24/11/2003  21:47            70 974 jis0212.enc
24/11/2003  21:47             1 091 koi8-r.enc
24/11/2003  21:47             1 091 koi8-u.enc
24/11/2003  21:47            92 877 ksc5601.enc
24/11/2003  21:47             1 096 macCentEuro.enc
24/11/2003  21:47             1 096 macCroatian.enc
24/11/2003  21:47             1 096 macCyrillic.enc
24/11/2003  21:47             1 096 macDingbats.enc
24/11/2003  21:47             1 093 macGreek.enc
24/11/2003  21:47             1 095 macIceland.enc
24/11/2003  21:47            48 028 macJapan.enc
24/11/2003  21:47             1 093 macRoman.enc
24/11/2003  21:47             1 095 macRomania.enc
24/11/2003  21:47             1 092 macThai.enc
24/11/2003  21:47             1 095 macTurkish.enc
24/11/2003  21:47             1 095 macUkraine.enc
24/11/2003  21:47            41 862 shiftjis.enc
24/11/2003  21:47             1 091 symbol.enc
24/11/2003  21:47             1 091 tis-620.enc
              78 fichier(s)        1 413 736 octets

 Répertoire de d:\scilab\lib\tcl8.4\http1.0

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47             9 759 http.tcl
24/11/2003  21:47               735 pkgIndex.tcl
               2 fichier(s)           10 494 octets

 Répertoire de d:\scilab\lib\tcl8.4\http2.4

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            23 865 http.tcl
24/11/2003  21:47               726 pkgIndex.tcl
               2 fichier(s)           24 591 octets

 Répertoire de d:\scilab\lib\tcl8.4\msgcat1.3

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            12 816 msgcat.tcl
24/11/2003  21:47               134 pkgIndex.tcl
               2 fichier(s)           12 950 octets

 Répertoire de d:\scilab\lib\tcl8.4\opt0.4

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            33 022 optparse.tcl
24/11/2003  21:47               609 pkgIndex.tcl
               2 fichier(s)           33 631 octets

 Répertoire de d:\scilab\lib\tcl8.4\tcltest2.2

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47               610 pkgIndex.tcl
24/11/2003  21:47            97 917 tcltest.tcl
               2 fichier(s)           98 527 octets

 Répertoire de d:\scilab\lib\tk8.4

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47             9 128 bgerror.tcl
24/11/2003  21:47            16 698 button.tcl
24/11/2003  21:47             8 985 choosedir.tcl
24/11/2003  21:47            21 366 clrpick.tcl
24/11/2003  21:47             7 698 comdlg.tcl
24/11/2003  21:47            26 942 console.tcl
13/02/2004  09:40    <REP>          demos
24/11/2003  21:47             6 827 dialog.tcl
24/11/2003  21:47            17 019 entry.tcl
24/11/2003  21:47             5 072 focus.tcl
13/02/2004  09:40    <REP>          images
24/11/2003  21:47            13 624 listbox.tcl
24/11/2003  21:47            37 050 menu.tcl
24/11/2003  21:47            27 260 mkpsenc.tcl
24/11/2003  21:47            15 934 msgbox.tcl
13/02/2004  09:40    <REP>          msgs
24/11/2003  21:47               803 obsolete.tcl
24/11/2003  21:47             1 659 optMenu.tcl
24/11/2003  21:47             8 004 palette.tcl
24/11/2003  21:47             5 040 panedwindow.tcl
24/11/2003  21:47               141 pkgIndex.tcl
24/11/2003  21:47             9 568 prolog.ps
24/11/2003  21:47             7 540 safetk.tcl
24/11/2003  21:47             7 866 scale.tcl
24/11/2003  21:47            11 766 scrlbar.tcl
24/11/2003  21:47            15 154 spinbox.tcl
24/11/2003  21:47            22 210 tclIndex
24/11/2003  21:47             4 675 tearoff.tcl
24/11/2003  21:47            30 884 text.tcl
24/11/2003  21:47            17 086 tk.tcl
24/11/2003  21:47             3 855 tkAppInit.c
24/11/2003  21:47            49 191 tkfbox.tcl
24/11/2003  21:47            11 462 unsupported.tcl
24/11/2003  21:47            25 258 xmfbox.tcl
              31 fichier(s)          445 765 octets

 Répertoire de d:\scilab\lib\tk8.4\demos

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47             8 138 arrow.tcl
24/11/2003  21:47             3 086 bind.tcl
24/11/2003  21:47             1 627 bitmap.tcl
24/11/2003  21:47             1 797 browse
24/11/2003  21:47             1 484 button.tcl
24/11/2003  21:47             1 369 check.tcl
24/11/2003  21:47             1 626 clrpick.tcl
24/11/2003  21:47             5 193 colors.tcl
24/11/2003  21:47             3 194 cscroll.tcl
24/11/2003  21:47             5 068 ctext.tcl
24/11/2003  21:47               731 dialog1.tcl
24/11/2003  21:47               680 dialog2.tcl
24/11/2003  21:47             1 583 entry1.tcl
24/11/2003  21:47             2 309 entry2.tcl
24/11/2003  21:47             6 134 entry3.tcl
24/11/2003  21:47             2 133 filebox.tcl
24/11/2003  21:47            79 359 floor.tcl
24/11/2003  21:47             1 242 form.tcl
24/11/2003  21:47               547 hello
24/11/2003  21:47             1 691 hscale.tcl
24/11/2003  21:47             2 206 icon.tcl
24/11/2003  21:47             1 148 image1.tcl
24/11/2003  21:47             3 355 image2.tcl
13/02/2004  09:40    <REP>          images
24/11/2003  21:47             9 864 items.tcl
24/11/2003  21:47             8 206 ixset
24/11/2003  21:47             1 462 label.tcl
24/11/2003  21:47             2 087 labelframe.tcl
24/11/2003  21:47             6 803 menu.tcl
24/11/2003  21:47             4 531 menubu.tcl
24/11/2003  21:47             2 175 msgbox.tcl
24/11/2003  21:47             1 304 paned1.tcl
24/11/2003  21:47             2 413 paned2.tcl
24/11/2003  21:47             2 950 plot.tcl
24/11/2003  21:47             2 801 puzzle.tcl
24/11/2003  21:47             2 412 radio.tcl
24/11/2003  21:47             2 135 README
24/11/2003  21:47             5 310 rmt
24/11/2003  21:47             8 109 rolodex
24/11/2003  21:47             5 369 ruler.tcl
24/11/2003  21:47             2 397 sayings.tcl
24/11/2003  21:47             4 596 search.tcl
24/11/2003  21:47             1 967 spin.tcl
24/11/2003  21:47             1 224 square
24/11/2003  21:47             1 865 states.tcl
24/11/2003  21:47             6 962 style.tcl
24/11/2003  21:47             4 354 tclIndex
24/11/2003  21:47            11 539 tcolor
24/11/2003  21:47             3 638 text.tcl
24/11/2003  21:47             1 128 timer
24/11/2003  21:47             6 753 twind.tcl
24/11/2003  21:47             2 769 unicodeout.tcl
24/11/2003  21:47             1 671 vscale.tcl
24/11/2003  21:47            12 557 widget.tcl
              53 fichier(s)          267 051 octets

 Répertoire de d:\scilab\lib\tk8.4\demos\images

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            51 712 earth.gif
24/11/2003  21:47             6 343 earthris.gif
24/11/2003  21:47            12 720 face.bmp
24/11/2003  21:47             1 886 flagdown.bmp
24/11/2003  21:47             1 880 flagup.bmp
24/11/2003  21:47               275 gray25.bmp
24/11/2003  21:47             1 883 letters.bmp
24/11/2003  21:47             1 889 noletter.bmp
24/11/2003  21:47               272 pattern.bmp
24/11/2003  21:47             2 341 tcllogo.gif
24/11/2003  21:47           196 623 teapot.ppm
              11 fichier(s)          277 824 octets

 Répertoire de d:\scilab\lib\tk8.4\images

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47            18 083 ActiveTclSplash.gif
24/11/2003  21:47            32 900 logo.eps
24/11/2003  21:47             2 341 logo100.gif
24/11/2003  21:47             1 670 logo64.gif
24/11/2003  21:47            11 000 logoLarge.gif
24/11/2003  21:47             3 889 logoMed.gif
24/11/2003  21:47            27 809 pwrdLogo.eps
24/11/2003  21:47             1 615 pwrdLogo100.gif
24/11/2003  21:47             2 489 pwrdLogo150.gif
24/11/2003  21:47             2 981 pwrdLogo175.gif
24/11/2003  21:47             3 491 pwrdLogo200.gif
24/11/2003  21:47             1 171 pwrdLogo75.gif
24/11/2003  21:47               388 README
24/11/2003  21:47             5 473 tai-ku.gif
24/11/2003  21:47             4 710 uninstall.ico
              15 fichier(s)          120 010 octets

 Répertoire de d:\scilab\lib\tk8.4\msgs

13/02/2004  09:40    <REP>          .
13/02/2004  09:40    <REP>          ..
24/11/2003  21:47             3 831 cs.msg
24/11/2003  21:47             3 728 de.msg
24/11/2003  21:47             8 610 el.msg
24/11/2003  21:47             2 540 en.msg
24/11/2003  21:47                63 en_gb.msg
24/11/2003  21:47             3 704 es.msg
24/11/2003  21:47             3 727 fr.msg
24/11/2003  21:47             3 544 it.msg
24/11/2003  21:47             7 095 nl.msg
24/11/2003  21:47             7 105 ru.msg
              10 fichier(s)           43 947 octets

     Total des fichiers listés :
             217 fichier(s)        2 870 372 octets
              38 Rép(s)  10 647 613 440 octets libres


And contents of the TCLLIB directory:

 Répertoire de d:\scilab\tcllib

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
13/02/2004  10:11    <REP>          include
               0 fichier(s)                0 octets

 Répertoire de d:\scilab\tcllib\include

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
24/11/2003  21:47             4 577 iconv.h
24/11/2003  21:47             6 454 itcl.h
24/11/2003  21:47             8 815 itclDecls.h
24/11/2003  21:47            11 159 itclInt.h
24/11/2003  21:47            41 825 itclIntDecls.h
24/11/2003  21:47             3 797 itk.h
24/11/2003  21:47             6 771 itkDecls.h
24/11/2003  21:47             4 802 jpegtcl.h
24/11/2003  21:47            16 568 jpegtclDecls.h
24/11/2003  21:47             3 111 jpegtclDeclsMask.h
24/11/2003  21:47             1 396 jpegtclDeclsUnmask.h
13/02/2004  10:11    <REP>          libexslt
13/02/2004  10:11    <REP>          libxml
13/02/2004  10:11    <REP>          libxslt
24/11/2003  21:47             4 780 pngtcl.h
24/11/2003  21:47           110 391 pngtclDecls.h
24/11/2003  21:47            16 752 pngtclDeclsMask.h
24/11/2003  21:47             8 083 pngtclDeclsUnmask.h
24/11/2003  21:47             1 221 proTbcLoad.h
24/11/2003  21:47            83 329 tcl.h
24/11/2003  21:47           158 651 tclDecls.h
24/11/2003  21:47             3 611 tcldom-libxml2.h
24/11/2003  21:47             3 876 tcldom.h
24/11/2003  21:47             1 348 tcldomDecls.h
24/11/2003  21:47             2 835 tcldomxmlDecls.h
24/11/2003  21:47             5 752 tclExtend.h
24/11/2003  21:47            94 049 tclInt.h
24/11/2003  21:47            49 247 tclIntDecls.h
24/11/2003  21:47            20 592 tclIntPlatDecls.h
24/11/2003  21:47             6 254 tclPlatDecls.h
24/11/2003  21:47            11 590 tclxml.h
24/11/2003  21:47             2 324 tclxslt.h
24/11/2003  21:47             4 802 tifftcl.h
24/11/2003  21:47            42 494 tifftclDecls.h
24/11/2003  21:47             7 491 tifftclDeclsMask.h
24/11/2003  21:47             3 538 tifftclDeclsUnmask.h
24/11/2003  21:47            57 193 tk.h
24/11/2003  21:47            90 179 tkDecls.h
24/11/2003  21:47             5 819 tkimg.h
24/11/2003  21:47             7 289 tkimgDecls.h
24/11/2003  21:47            93 044 tkIntXlibDecls.h
24/11/2003  21:47            10 202 tkPlatDecls.h
24/11/2003  21:47            25 171 tkTable.h
24/11/2003  21:47            31 570 transform.h
24/11/2003  21:47             4 181 trfDecls.h
13/02/2004  10:11    <REP>          X11
24/11/2003  21:47            20 829 xmlparse.h
24/11/2003  21:47             3 836 xmlrole.h
24/11/2003  21:47            11 189 xmltok.h
24/11/2003  21:47             4 798 zlibtcl.h
24/11/2003  21:47            11 312 zlibtclDecls.h
24/11/2003  21:47             1 765 zlibtclDeclsMask.h
24/11/2003  21:47               728 zlibtclDeclsUnmask.h
              49 fichier(s)        1 131 390 octets

 Répertoire de d:\scilab\tcllib\include\libexslt

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
24/11/2003  21:47             1 446 exslt.h
24/11/2003  21:47             1 594 exsltconfig.h
24/11/2003  21:47             1 594 exsltwin32config.h
               3 fichier(s)            4 634 octets

 Répertoire de d:\scilab\tcllib\include\libxml

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
24/11/2003  21:47             2 218 c14n.h
24/11/2003  21:47             4 049 catalog.h
24/11/2003  21:47             3 909 debugXML.h
24/11/2003  21:47             2 777 DOCBparser.h
24/11/2003  21:47             7 849 encoding.h
24/11/2003  21:47             3 460 entities.h
24/11/2003  21:47             8 468 globals.h
24/11/2003  21:47             5 028 hash.h
24/11/2003  21:47             3 541 HTMLparser.h
24/11/2003  21:47             2 825 HTMLtree.h
24/11/2003  21:47             3 241 list.h
24/11/2003  21:47               776 Makefile.am
24/11/2003  21:47             8 626 Makefile.in
24/11/2003  21:47             2 842 nanoftp.h
24/11/2003  21:47             1 440 nanohttp.h
24/11/2003  21:47            27 196 parser.h
24/11/2003  21:47            12 068 parserInternals.h
24/11/2003  21:47             3 488 SAX.h
24/11/2003  21:47             6 998 schemasInternals.h
24/11/2003  21:47             1 216 threads.h
24/11/2003  21:47            25 502 tree.h
24/11/2003  21:47             1 746 uri.h
24/11/2003  21:47             9 329 valid.h
24/11/2003  21:47               471 xinclude.h
24/11/2003  21:47             4 929 xlink.h
24/11/2003  21:47             2 577 xmlautomata.h
24/11/2003  21:47             4 617 xmlerror.h
24/11/2003  21:47             8 268 xmlIO.h
24/11/2003  21:47             4 135 xmlmemory.h
24/11/2003  21:47             1 735 xmlregexp.h
24/11/2003  21:47             2 950 xmlschemas.h
24/11/2003  21:47             1 082 xmlschemastypes.h
24/11/2003  21:47             5 571 xmlunicode.h
24/11/2003  21:47             5 112 xmlversion.h
24/11/2003  21:47             5 106 xmlversion.h.in
24/11/2003  21:47             5 112 xmlwin32version.h
24/11/2003  21:47             4 511 xmlwin32version.h.in
24/11/2003  21:47            12 369 xpath.h
24/11/2003  21:47            17 807 xpathInternals.h
24/11/2003  21:47             2 531 xpointer.h
              40 fichier(s)          237 475 octets

 Répertoire de d:\scilab\tcllib\include\libxslt

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
24/11/2003  21:47               108 acconfig.h
24/11/2003  21:47               694 attributes.h
24/11/2003  21:47               919 documents.h
24/11/2003  21:47             5 605 extensions.h
24/11/2003  21:47             1 585 extra.h
24/11/2003  21:47             1 700 functions.h
24/11/2003  21:47             1 571 imports.h
24/11/2003  21:47               862 keys.h
24/11/2003  21:47               716 libxslt.h
24/11/2003  21:47               979 namespaces.h
24/11/2003  21:47             1 628 numbersInternals.h
24/11/2003  21:47             1 528 pattern.h
24/11/2003  21:47               667 preproc.h
24/11/2003  21:47             1 601 templates.h
24/11/2003  21:47             4 964 transform.h
24/11/2003  21:47             2 206 variables.h
24/11/2003  21:47             2 052 win32config.h
24/11/2003  21:47             1 745 xslt.h
24/11/2003  21:47             2 383 xsltconfig.h
24/11/2003  21:47            16 141 xsltInternals.h
24/11/2003  21:47             5 057 xsltutils.h
24/11/2003  21:47             2 383 xsltwin32config.h
              22 fichier(s)           57 094 octets

 Répertoire de d:\scilab\tcllib\include\X11

13/02/2004  10:11    <REP>          .
13/02/2004  10:11    <REP>          ..
24/11/2003  21:47             1 953 cursorfont.h
24/11/2003  21:47             1 501 keysym.h
24/11/2003  21:47            52 429 keysymdef.h
24/11/2003  21:47            18 697 X.h
24/11/2003  21:47             2 518 Xatom.h
24/11/2003  21:47             1 720 Xfuncproto.h
24/11/2003  21:47            40 988 Xlib.h
24/11/2003  21:47            20 273 Xutil.h
               8 fichier(s)          140 079 octets

 