How to compile with visual studio C++ .Net 2003 and Intel Fortran 8.0 ?

Pre-requirements:

Install Visual Studio C++ .NET 2003
Install Intel Fortran 8.0
Install TCL/TK 8.4(.6)


1] copy tcl84.dll,tk84.dll,tcl84.lib and tk84.lib in bin directory.
2] edit Make-PVM3.mak in pvm3 directory
   and modify first line (PVM_ROOT=...) with the good path
3] execute configheaders.bat
4] copy in root of scilab directory : "tcl" directory (see below)
5] click on scilab.sln (to launch Visual) or devenv scilab.sln /build release (via command line, to build release)


N.B:
if you have this message : "fatal error LNK1127: library is corrupt"
disable your antivirus and rebuild.

contents of the "tcl" directory:

  Répertoire de D:\Scilab\bin

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15           798 720 tcl84.dll
02/03/2004  08:15           604 428 tcl84.lib
02/03/2004  08:15         1 032 192 tk84.dll
02/03/2004  08:15           529 946 tk84.lib
               4 fichier(s)        2 965 286 octets

 Répertoire de D:\Scilab\tcl

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
05/05/2004  08:28    <REP>          include
05/05/2004  08:28    <REP>          tcl8.4
05/05/2004  08:28    <REP>          tk8.4
               0 fichier(s)                0 octets

 Répertoire de D:\Scilab\tcl\include

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             4 577 iconv.h
02/03/2004  08:15             6 454 itcl.h
02/03/2004  08:15             8 815 itclDecls.h
02/03/2004  08:15            11 159 itclInt.h
02/03/2004  08:15            41 825 itclIntDecls.h
02/03/2004  08:15             3 797 itk.h
02/03/2004  08:15             6 771 itkDecls.h
02/03/2004  08:15             4 802 jpegtcl.h
02/03/2004  08:15            16 568 jpegtclDecls.h
02/03/2004  08:15             3 111 jpegtclDeclsMask.h
02/03/2004  08:15             1 396 jpegtclDeclsUnmask.h
05/05/2004  08:28    <REP>          libexslt
05/05/2004  08:28    <REP>          libxml
05/05/2004  08:28    <REP>          libxslt
02/03/2004  08:15             4 780 pngtcl.h
02/03/2004  08:15           110 391 pngtclDecls.h
02/03/2004  08:15            16 752 pngtclDeclsMask.h
02/03/2004  08:15             8 083 pngtclDeclsUnmask.h
02/03/2004  08:15             1 221 proTbcLoad.h
02/03/2004  08:15            83 320 tcl.h
02/03/2004  08:15           158 651 tclDecls.h
02/03/2004  08:15             3 611 tcldom-libxml2.h
02/03/2004  08:15             3 876 tcldom.h
02/03/2004  08:15             1 348 tcldomDecls.h
02/03/2004  08:15             2 835 tcldomxmlDecls.h
02/03/2004  08:15             5 752 tclExtend.h
02/03/2004  08:15            94 049 tclInt.h
02/03/2004  08:15            49 247 tclIntDecls.h
02/03/2004  08:15            20 592 tclIntPlatDecls.h
02/03/2004  08:15             6 254 tclPlatDecls.h
02/03/2004  08:15            11 590 tclxml.h
02/03/2004  08:15             2 324 tclxslt.h
02/03/2004  08:15             4 802 tifftcl.h
02/03/2004  08:15            42 494 tifftclDecls.h
02/03/2004  08:15             7 491 tifftclDeclsMask.h
02/03/2004  08:15             3 538 tifftclDeclsUnmask.h
02/03/2004  08:15            57 195 tk.h
02/03/2004  08:15            90 179 tkDecls.h
02/03/2004  08:15             5 819 tkimg.h
02/03/2004  08:15             7 289 tkimgDecls.h
02/03/2004  08:15            93 044 tkIntXlibDecls.h
02/03/2004  08:15            10 202 tkPlatDecls.h
02/03/2004  08:15            24 283 tkTable.h
02/03/2004  08:15            31 570 transform.h
02/03/2004  08:15             4 181 trfDecls.h
05/05/2004  08:28    <REP>          X11
02/03/2004  08:15            20 829 xmlparse.h
02/03/2004  08:15             3 836 xmlrole.h
02/03/2004  08:15            11 189 xmltok.h
02/03/2004  08:15             4 798 zlibtcl.h
02/03/2004  08:15            11 312 zlibtclDecls.h
02/03/2004  08:15             1 765 zlibtclDeclsMask.h
02/03/2004  08:15               728 zlibtclDeclsUnmask.h
              49 fichier(s)        1 130 495 octets

 Répertoire de D:\Scilab\tcl\include\libexslt

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             1 446 exslt.h
02/03/2004  08:15             1 594 exsltconfig.h
02/03/2004  08:15             1 594 exsltwin32config.h
               3 fichier(s)            4 634 octets

 Répertoire de D:\Scilab\tcl\include\libxml

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             2 218 c14n.h
02/03/2004  08:15             4 049 catalog.h
02/03/2004  08:15             3 909 debugXML.h
02/03/2004  08:15             2 777 DOCBparser.h
02/03/2004  08:15             7 849 encoding.h
02/03/2004  08:15             3 460 entities.h
02/03/2004  08:15             8 468 globals.h
02/03/2004  08:15             5 028 hash.h
02/03/2004  08:15             3 541 HTMLparser.h
02/03/2004  08:15             2 825 HTMLtree.h
02/03/2004  08:15             3 241 list.h
02/03/2004  08:15               776 Makefile.am
02/03/2004  08:15             8 626 Makefile.in
02/03/2004  08:15             2 842 nanoftp.h
02/03/2004  08:15             1 440 nanohttp.h
02/03/2004  08:15            27 196 parser.h
02/03/2004  08:15            12 068 parserInternals.h
02/03/2004  08:15             3 488 SAX.h
02/03/2004  08:15             6 998 schemasInternals.h
02/03/2004  08:15             1 216 threads.h
02/03/2004  08:15            25 502 tree.h
02/03/2004  08:15             1 746 uri.h
02/03/2004  08:15             9 329 valid.h
02/03/2004  08:15               471 xinclude.h
02/03/2004  08:15             4 929 xlink.h
02/03/2004  08:15             2 577 xmlautomata.h
02/03/2004  08:15             4 617 xmlerror.h
02/03/2004  08:15             8 268 xmlIO.h
02/03/2004  08:15             4 135 xmlmemory.h
02/03/2004  08:15             1 735 xmlregexp.h
02/03/2004  08:15             2 950 xmlschemas.h
02/03/2004  08:15             1 082 xmlschemastypes.h
02/03/2004  08:15             5 571 xmlunicode.h
02/03/2004  08:15             5 112 xmlversion.h
02/03/2004  08:15             5 106 xmlversion.h.in
02/03/2004  08:15             5 112 xmlwin32version.h
02/03/2004  08:15             4 511 xmlwin32version.h.in
02/03/2004  08:15            12 369 xpath.h
02/03/2004  08:15            17 807 xpathInternals.h
02/03/2004  08:15             2 531 xpointer.h
              40 fichier(s)          237 475 octets

 Répertoire de D:\Scilab\tcl\include\libxslt

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15               108 acconfig.h
02/03/2004  08:15               694 attributes.h
02/03/2004  08:15               919 documents.h
02/03/2004  08:15             5 605 extensions.h
02/03/2004  08:15             1 585 extra.h
02/03/2004  08:15             1 700 functions.h
02/03/2004  08:15             1 571 imports.h
02/03/2004  08:15               862 keys.h
02/03/2004  08:15               716 libxslt.h
02/03/2004  08:15               979 namespaces.h
02/03/2004  08:15             1 628 numbersInternals.h
02/03/2004  08:15             1 528 pattern.h
02/03/2004  08:15               667 preproc.h
02/03/2004  08:15             1 601 templates.h
02/03/2004  08:15             4 964 transform.h
02/03/2004  08:15             2 206 variables.h
02/03/2004  08:15             2 052 win32config.h
02/03/2004  08:15             1 745 xslt.h
02/03/2004  08:15             2 383 xsltconfig.h
02/03/2004  08:15            16 141 xsltInternals.h
02/03/2004  08:15             5 057 xsltutils.h
02/03/2004  08:15             2 383 xsltwin32config.h
              22 fichier(s)           57 094 octets

 Répertoire de D:\Scilab\tcl\include\X11

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             1 953 cursorfont.h
02/03/2004  08:15             1 501 keysym.h
02/03/2004  08:15            52 429 keysymdef.h
02/03/2004  08:15            18 697 X.h
02/03/2004  08:15             2 518 Xatom.h
02/03/2004  08:15             1 720 Xfuncproto.h
02/03/2004  08:15            40 988 Xlib.h
02/03/2004  08:15            20 273 Xutil.h
               8 fichier(s)          140 079 octets

 Répertoire de D:\Scilab\tcl\tcl8.4

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            19 529 auto.tcl
05/05/2004  08:28    <REP>          encoding
02/03/2004  08:15             9 108 history.tcl
05/05/2004  08:28    <REP>          http1.0
05/05/2004  08:28    <REP>          http2.4
02/03/2004  08:16            23 427 init.tcl
02/03/2004  08:15             6 802 ldAout.tcl
05/05/2004  08:28    <REP>          msgcat1.3
05/05/2004  08:28    <REP>          opt0.4
02/03/2004  08:15            24 008 package.tcl
02/03/2004  08:15               882 parray.tcl
02/03/2004  08:15            27 614 safe.tcl
02/03/2004  08:15             6 097 tclIndex
05/05/2004  08:28    <REP>          tcltest2.2
02/03/2004  08:15             4 375 word.tcl
               9 fichier(s)          121 842 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\encoding

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             1 090 ascii.enc
02/03/2004  08:15            92 873 big5.enc
02/03/2004  08:15             1 091 cp1250.enc
02/03/2004  08:15             1 091 cp1251.enc
02/03/2004  08:15             1 091 cp1252.enc
02/03/2004  08:15             1 091 cp1253.enc
02/03/2004  08:15             1 091 cp1254.enc
02/03/2004  08:15             1 091 cp1255.enc
02/03/2004  08:15             1 091 cp1256.enc
02/03/2004  08:15             1 091 cp1257.enc
02/03/2004  08:15             1 091 cp1258.enc
02/03/2004  08:15             1 090 cp437.enc
02/03/2004  08:15             1 090 cp737.enc
02/03/2004  08:15             1 090 cp775.enc
02/03/2004  08:15             1 090 cp850.enc
02/03/2004  08:15             1 090 cp852.enc
02/03/2004  08:15             1 090 cp855.enc
02/03/2004  08:15             1 090 cp857.enc
02/03/2004  08:15             1 090 cp860.enc
02/03/2004  08:15             1 090 cp861.enc
02/03/2004  08:15             1 090 cp862.enc
02/03/2004  08:15             1 090 cp863.enc
02/03/2004  08:15             1 090 cp864.enc
02/03/2004  08:15             1 090 cp865.enc
02/03/2004  08:15             1 090 cp866.enc
02/03/2004  08:15             1 090 cp869.enc
02/03/2004  08:15             1 090 cp874.enc
02/03/2004  08:15            48 207 cp932.enc
02/03/2004  08:15           132 509 cp936.enc
02/03/2004  08:15           130 423 cp949.enc
02/03/2004  08:15            91 831 cp950.enc
02/03/2004  08:15             1 093 dingbats.enc
02/03/2004  08:15             1 054 ebcdic.enc
02/03/2004  08:15            85 574 euc-cn.enc
02/03/2004  08:15            82 537 euc-jp.enc
02/03/2004  08:15            93 918 euc-kr.enc
02/03/2004  08:15            86 619 gb12345.enc
02/03/2004  08:15             1 091 gb1988.enc
02/03/2004  08:15            84 532 gb2312-raw.enc
02/03/2004  08:15            85 574 gb2312.enc
02/03/2004  08:15               192 iso2022-jp.enc
02/03/2004  08:15               115 iso2022-kr.enc
02/03/2004  08:15               226 iso2022.enc
02/03/2004  08:15             1 094 iso8859-1.enc
02/03/2004  08:15             1 095 iso8859-10.enc
02/03/2004  08:15             1 095 iso8859-13.enc
02/03/2004  08:15             1 095 iso8859-14.enc
02/03/2004  08:15             1 095 iso8859-15.enc
02/03/2004  08:15             1 095 iso8859-16.enc
02/03/2004  08:15             1 094 iso8859-2.enc
02/03/2004  08:15             1 094 iso8859-3.enc
02/03/2004  08:15             1 094 iso8859-4.enc
02/03/2004  08:15             1 094 iso8859-5.enc
02/03/2004  08:15             1 094 iso8859-6.enc
02/03/2004  08:15             1 094 iso8859-7.enc
02/03/2004  08:15             1 094 iso8859-8.enc
02/03/2004  08:15             1 094 iso8859-9.enc
02/03/2004  08:15             1 092 jis0201.enc
02/03/2004  08:15            80 459 jis0208.enc
02/03/2004  08:15            70 974 jis0212.enc
02/03/2004  08:15             1 091 koi8-r.enc
02/03/2004  08:15             1 091 koi8-u.enc
02/03/2004  08:15            92 877 ksc5601.enc
02/03/2004  08:15             1 096 macCentEuro.enc
02/03/2004  08:15             1 096 macCroatian.enc
02/03/2004  08:15             1 096 macCyrillic.enc
02/03/2004  08:15             1 096 macDingbats.enc
02/03/2004  08:15             1 093 macGreek.enc
02/03/2004  08:15             1 095 macIceland.enc
02/03/2004  08:15            48 028 macJapan.enc
02/03/2004  08:15             1 093 macRoman.enc
02/03/2004  08:15             1 095 macRomania.enc
02/03/2004  08:15             1 092 macThai.enc
02/03/2004  08:15             1 095 macTurkish.enc
02/03/2004  08:15             1 095 macUkraine.enc
02/03/2004  08:15            41 862 shiftjis.enc
02/03/2004  08:15             1 091 symbol.enc
02/03/2004  08:15             1 091 tis-620.enc
              78 fichier(s)        1 413 736 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\http1.0

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             9 759 http.tcl
02/03/2004  08:15               735 pkgIndex.tcl
               2 fichier(s)           10 494 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\http2.4

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            23 865 http.tcl
02/03/2004  08:15               726 pkgIndex.tcl
               2 fichier(s)           24 591 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\msgcat1.3

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            12 816 msgcat.tcl
02/03/2004  08:15               134 pkgIndex.tcl
               2 fichier(s)           12 950 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\opt0.4

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            33 022 optparse.tcl
02/03/2004  08:15               609 pkgIndex.tcl
               2 fichier(s)           33 631 octets

 Répertoire de D:\Scilab\tcl\tcl8.4\tcltest2.2

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15               610 pkgIndex.tcl
02/03/2004  08:15            98 064 tcltest.tcl
               2 fichier(s)           98 674 octets

 Répertoire de D:\Scilab\tcl\tk8.4

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             9 128 bgerror.tcl
02/03/2004  08:15            16 698 button.tcl
02/03/2004  08:15             8 985 choosedir.tcl
02/03/2004  08:15            21 366 clrpick.tcl
02/03/2004  08:15             7 698 comdlg.tcl
02/03/2004  08:15            26 942 console.tcl
05/05/2004  08:28    <REP>          demos
02/03/2004  08:15             6 827 dialog.tcl
02/03/2004  08:15            17 019 entry.tcl
02/03/2004  08:15             5 072 focus.tcl
05/05/2004  08:28    <REP>          images
02/03/2004  08:15            14 121 listbox.tcl
02/03/2004  08:15            37 364 menu.tcl
02/03/2004  08:15            27 260 mkpsenc.tcl
02/03/2004  08:15            15 934 msgbox.tcl
05/05/2004  08:28    <REP>          msgs
02/03/2004  08:15               803 obsolete.tcl
02/03/2004  08:15             1 659 optMenu.tcl
02/03/2004  08:15             8 004 palette.tcl
02/03/2004  08:15             5 040 panedwindow.tcl
02/03/2004  08:15               141 pkgIndex.tcl
02/03/2004  08:15             9 568 prolog.ps
02/03/2004  08:15             7 540 safetk.tcl
02/03/2004  08:15             7 866 scale.tcl
02/03/2004  08:15            12 254 scrlbar.tcl
02/03/2004  08:15            15 154 spinbox.tcl
02/03/2004  08:15            22 210 tclIndex
02/03/2004  08:15             4 675 tearoff.tcl
02/03/2004  08:15            31 369 text.tcl
02/03/2004  08:15            17 086 tk.tcl
02/03/2004  08:15             3 855 tkAppInit.c
02/03/2004  08:15            49 199 tkfbox.tcl
02/03/2004  08:15            11 462 unsupported.tcl
02/03/2004  08:15            25 258 xmfbox.tcl
              31 fichier(s)          447 557 octets

 Répertoire de D:\Scilab\tcl\tk8.4\demos

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             8 138 arrow.tcl
02/03/2004  08:15             3 086 bind.tcl
02/03/2004  08:15             1 627 bitmap.tcl
02/03/2004  08:15             1 797 browse
02/03/2004  08:15             1 484 button.tcl
02/03/2004  08:15             1 369 check.tcl
02/03/2004  08:15             1 626 clrpick.tcl
02/03/2004  08:15             5 193 colors.tcl
02/03/2004  08:15             3 194 cscroll.tcl
02/03/2004  08:15             5 068 ctext.tcl
02/03/2004  08:15               731 dialog1.tcl
02/03/2004  08:15               680 dialog2.tcl
02/03/2004  08:15             1 583 entry1.tcl
02/03/2004  08:15             2 309 entry2.tcl
02/03/2004  08:15             6 134 entry3.tcl
02/03/2004  08:15             2 133 filebox.tcl
02/03/2004  08:15            79 359 floor.tcl
02/03/2004  08:15             1 242 form.tcl
02/03/2004  08:15               547 hello
02/03/2004  08:15             1 691 hscale.tcl
02/03/2004  08:15             2 206 icon.tcl
02/03/2004  08:15             1 148 image1.tcl
02/03/2004  08:15             3 355 image2.tcl
05/05/2004  08:28    <REP>          images
02/03/2004  08:15             9 864 items.tcl
02/03/2004  08:15             8 206 ixset
02/03/2004  08:15             1 462 label.tcl
02/03/2004  08:15             2 087 labelframe.tcl
02/03/2004  08:15             6 803 menu.tcl
02/03/2004  08:15             4 531 menubu.tcl
02/03/2004  08:15             2 175 msgbox.tcl
02/03/2004  08:15             1 304 paned1.tcl
02/03/2004  08:15             2 413 paned2.tcl
02/03/2004  08:15             2 950 plot.tcl
02/03/2004  08:15             2 801 puzzle.tcl
02/03/2004  08:15             2 412 radio.tcl
02/03/2004  08:15             2 135 README
02/03/2004  08:15             5 310 rmt
02/03/2004  08:15             8 109 rolodex
02/03/2004  08:15             5 369 ruler.tcl
02/03/2004  08:15             2 397 sayings.tcl
02/03/2004  08:15             4 596 search.tcl
02/03/2004  08:15             1 967 spin.tcl
02/03/2004  08:15             1 224 square
02/03/2004  08:15             1 865 states.tcl
02/03/2004  08:15             6 962 style.tcl
02/03/2004  08:15             4 354 tclIndex
02/03/2004  08:15            11 539 tcolor
02/03/2004  08:15             3 638 text.tcl
02/03/2004  08:15             1 128 timer
02/03/2004  08:15             6 753 twind.tcl
02/03/2004  08:15             2 769 unicodeout.tcl
02/03/2004  08:15             1 671 vscale.tcl
02/03/2004  08:15            12 557 widget.tcl
              53 fichier(s)          267 051 octets

 Répertoire de D:\Scilab\tcl\tk8.4\demos\images

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            51 712 earth.gif
02/03/2004  08:15             6 343 earthris.gif
02/03/2004  08:15            12 720 face.bmp
02/03/2004  08:15             1 886 flagdown.bmp
02/03/2004  08:15             1 880 flagup.bmp
02/03/2004  08:15               275 gray25.bmp
02/03/2004  08:15             1 883 letters.bmp
02/03/2004  08:15             1 889 noletter.bmp
02/03/2004  08:15               272 pattern.bmp
02/03/2004  08:15             2 341 tcllogo.gif
02/03/2004  08:15           196 623 teapot.ppm
              11 fichier(s)          277 824 octets

 Répertoire de D:\Scilab\tcl\tk8.4\images

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15            18 083 ActiveTclSplash.gif
02/03/2004  08:15            32 900 logo.eps
02/03/2004  08:15             2 341 logo100.gif
02/03/2004  08:15             1 670 logo64.gif
02/03/2004  08:15            11 000 logoLarge.gif
02/03/2004  08:15             3 889 logoMed.gif
02/03/2004  08:15            27 809 pwrdLogo.eps
02/03/2004  08:15             1 615 pwrdLogo100.gif
02/03/2004  08:15             2 489 pwrdLogo150.gif
02/03/2004  08:15             2 981 pwrdLogo175.gif
02/03/2004  08:15             3 491 pwrdLogo200.gif
02/03/2004  08:15             1 171 pwrdLogo75.gif
02/03/2004  08:15               388 README
02/03/2004  08:15             5 473 tai-ku.gif
02/03/2004  08:15             4 710 uninstall.ico
              15 fichier(s)          120 010 octets

 Répertoire de D:\Scilab\tcl\tk8.4\msgs

05/05/2004  08:28    <REP>          .
05/05/2004  08:28    <REP>          ..
02/03/2004  08:15             3 831 cs.msg
02/03/2004  08:15             3 728 de.msg
02/03/2004  08:15             8 610 el.msg
02/03/2004  08:15             2 540 en.msg
02/03/2004  08:15                63 en_gb.msg
02/03/2004  08:15             3 845 eo.msg
02/03/2004  08:15             3 704 es.msg
02/03/2004  08:15             3 727 fr.msg
02/03/2004  08:15             3 544 it.msg
02/03/2004  08:15             7 095 nl.msg
02/03/2004  08:15             3 948 pl.msg
02/03/2004  08:15             7 105 ru.msg
              12 fichier(s)           51 740 octets
