      subroutine lstelm
c ================================== ( Inria    ) =============
c
c     evaluate utility list's functions
c
c =============================================================
c     

c     Copyright INRIA
c     
      include '../stack.h'
      integer ltyp
      integer iadr,sadr
c
      external rattyp,lsstyp,gratyp
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lstelm '//buf(1:4))
      endif
c
c     functions/fin
c     1        2       3       4        5     6     7     8
c     list     tlist   rlist   lsslist glist lstcat mlist definedfields
c     9
c     lstsize

c
      rhs=max(0,rhs)
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
c
      goto(01,01,20,30,40,50,01,60,70) fin
c
 01   lf=lstk(top+1)
      top=top+1-rhs
      ld=lstk(top)
c
      n=rhs
      err=lf+sadr(n+3)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(fin.eq.2) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=16
      elseif(fin.eq.7) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=17
      else
c     c untyped list
         ltyp=15
      endif

      il=iadr(ld)
      l=sadr(il+n+3)
      call unsfdcopy(lf-ld,stk(ld),-1,stk(l),-1)

      istk(il)=ltyp
      istk(il+1)=n
      istk(il+2)=1
      if(n.ne.0) then
         do 10 i=1,n
            istk(il+2+i)=istk(il+1+i)+lstk(top+i)-lstk(top-1+i)
 10      continue
      endif
      lstk(top+1)=l+lf-ld
      goto 99
c
 20   continue
c     rlist
      call mtlist(rattyp)
      if(err.gt.0) return
      goto 99
c
 30   continue
c     lsslist
      call mtlist(lsstyp)
      if(err.gt.0) return
      goto 99
c
 40   continue
c     glist
      call mtlist(gratyp)
      if(err.gt.0) return
      goto 99

 50   continue
c     lstcat
      call intlstcat()
      if(err.gt.0) return
      goto 99

 60   continue
c     definedfields
      call definedfields()
      if(err.gt.0) return
      goto 99

 70   continue
c     lstsize
      call intlstsize()
      if(err.gt.0) return
      goto 99


 99   return
      end

      subroutine mtlist(typdsc)
c     create a tlist whose type description is given by external
c     function typdsc
c
      include '../stack.h'
      integer iadr,sadr
      external typdsc
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lf=lstk(top+1)
      top=top+1-rhs
      ld=lstk(top)
c     get the size of type description variable
      call typdsc(n1,'size')

      err=lf+sadr(rhs+4+n1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      il=iadr(ld)
      l1=sadr(il+rhs+4)
      ilt=iadr(l1)
      l=sadr(ilt+n1)
      call unsfdcopy(lf-ld,stk(ld),-1,stk(l),-1)
      istk(il)=16
      istk(il+1)=rhs+1
c     set the type
      call typdsc(istk(ilt),'set')
      nt=sadr(ilt+n1)-l1
c     set the pointers
      istk(il+2)=1
      istk(il+3)=nt+1
      do 41 i=1,rhs
         istk(il+3+i)=istk(il+2+i)+lstk(top+i)-lstk(top-1+i)
 41   continue
      lstk(top+1)=l+lf-ld
      return
      end

      subroutine lsstyp(ivt,job)
c     set the type
      integer ivt(*)
      character*(*) job
      if(job.eq.'size') then
         ivt(1)=23
      else
         ivt(1)=10
         ivt(2)=1
         ivt(3)=7
         ivt(4)=0
         ivt(5)=1
         ivt(6)=4
         ivt(7)=5
         ivt(8)=6
         ivt(9)=7
         ivt(10)=8
         ivt(11)=10
         ivt(12)=12
         ivt(13)=21
         ivt(14)=28
         ivt(15)=28
         ivt(16)=-10
         ivt(17)=-11
         ivt(18)=-12
         ivt(19)=-13
         ivt(20)=-33
         ivt(21)=0
         ivt(22)=13
         ivt(23)=29
      endif
      return
      end


      subroutine rattyp(ivt,job)
c     set the type ['r','num','den','dt']
      integer ivt(*)
c
      character*(*) job
      if(job.eq.'size') then
         ivt(1)=18
      else
         ivt(1)=10
         ivt(2)=1
         ivt(3)=4
         ivt(4)=0
         ivt(5)=1
         ivt(6)=2
         ivt(7)=5
         ivt(8)=8
         ivt(9)=10
         ivt(10)=27
         ivt(11)=23
         ivt(12)=30
         ivt(13)=22
         ivt(14)=13
         ivt(15)=14
         ivt(16)=23
         ivt(17)=13
         ivt(18)=29
      endif
      return
      end
      subroutine gratyp(ivt,job)
c     set the type  
c      ['graph','name','directed','node_number','tail','head','node_name',..
c       'node_type','node_x','node_y','node_color','node_diam','node_border',..
c       'node_font_size','node_demand','edge_name','edge_color','edge_width',..
c       'edge_hi_width','edge_font_size','edge_length','edge_cost',..
c       'edge_min_cap','edge_max_cap','edge_q_weight','edge_q_orig',..
c       'edge_weight','default_node_diam','default_node_border',..
c       'default_edge_width','default_edge_hi_width','default_font_size',..
c       'node_label','edge_label']

      integer ivt(*)
      character*(*) job
      if(job.eq.'size') then
         ivt(1)=407
      else
         ivt(1)=10
         ivt(2)=1
         ivt(3)=34
         ivt(4)=0
         ivt(5)=1
         ivt(6)=6
         ivt(7)=10
         ivt(8)=18
         ivt(9)=29
         ivt(10)=33
         ivt(11)=37
         ivt(12)=46
         ivt(13)=55
         ivt(14)=61
         ivt(15)=67
         ivt(16)=77
         ivt(17)=86
         ivt(18)=97
         ivt(19)=111
         ivt(20)=122
         ivt(21)=131
         ivt(22)=141
         ivt(23)=151
         ivt(24)=164
         ivt(25)=178
         ivt(26)=189
         ivt(27)=198
         ivt(28)=210
         ivt(29)=222
         ivt(30)=235
         ivt(31)=246
         ivt(32)=257
         ivt(33)=274
         ivt(34)=293
         ivt(35)=311
         ivt(36)=332
         ivt(37)=349
         ivt(38)=359
         ivt(39)=369
         ivt(40)=16
         ivt(41)=27
         ivt(42)=10
         ivt(43)=25
         ivt(44)=17
         ivt(45)=23
         ivt(46)=10
         ivt(47)=22
         ivt(48)=14
         ivt(49)=13
         ivt(50)=18
         ivt(51)=27
         ivt(52)=14
         ivt(53)=12
         ivt(54)=29
         ivt(55)=14
         ivt(56)=13
         ivt(57)=23
         ivt(58)=24
         ivt(59)=13
         ivt(60)=14
         ivt(61)=36
         ivt(62)=23
         ivt(63)=30
         ivt(64)=22
         ivt(65)=11
         ivt(66)=14
         ivt(67)=27
         ivt(68)=29
         ivt(69)=10
         ivt(70)=18
         ivt(71)=21
         ivt(72)=17
         ivt(73)=14
         ivt(74)=10
         ivt(75)=13
         ivt(76)=23
         ivt(77)=24
         ivt(78)=13
         ivt(79)=14
         ivt(80)=36
         ivt(81)=23
         ivt(82)=10
         ivt(83)=22
         ivt(84)=14
         ivt(85)=23
         ivt(86)=24
         ivt(87)=13
         ivt(88)=14
         ivt(89)=36
         ivt(90)=29
         ivt(91)=34
         ivt(92)=25
         ivt(93)=14
         ivt(94)=23
         ivt(95)=24
         ivt(96)=13
         ivt(97)=14
         ivt(98)=36
         ivt(99)=33
         ivt(100)=23
         ivt(101)=24
         ivt(102)=13
         ivt(103)=14
         ivt(104)=36
         ivt(105)=34
         ivt(106)=23
         ivt(107)=24
         ivt(108)=13
         ivt(109)=14
         ivt(110)=36
         ivt(111)=12
         ivt(112)=24
         ivt(113)=21
         ivt(114)=24
         ivt(115)=27
         ivt(116)=23
         ivt(117)=24
         ivt(118)=13
         ivt(119)=14
         ivt(120)=36
         ivt(121)=13
         ivt(122)=18
         ivt(123)=10
         ivt(124)=22
         ivt(125)=23
         ivt(126)=24
         ivt(127)=13
         ivt(128)=14
         ivt(129)=36
         ivt(130)=11
         ivt(131)=24
         ivt(132)=27
         ivt(133)=13
         ivt(134)=14
         ivt(135)=27
         ivt(136)=23
         ivt(137)=24
         ivt(138)=13
         ivt(139)=14
         ivt(140)=36
         ivt(141)=15
         ivt(142)=24
         ivt(143)=23
         ivt(144)=29
         ivt(145)=36
         ivt(146)=28
         ivt(147)=18
         ivt(148)=35
         ivt(149)=14
         ivt(150)=23
         ivt(151)=24
         ivt(152)=13
         ivt(153)=14
         ivt(154)=36
         ivt(155)=13
         ivt(156)=14
         ivt(157)=22
         ivt(158)=10
         ivt(159)=23
         ivt(160)=13
         ivt(161)=14
         ivt(162)=13
         ivt(163)=16
         ivt(164)=14
         ivt(165)=36
         ivt(166)=23
         ivt(167)=10
         ivt(168)=22
         ivt(169)=14
         ivt(170)=14
         ivt(171)=13
         ivt(172)=16
         ivt(173)=14
         ivt(174)=36
         ivt(175)=12
         ivt(176)=24
         ivt(177)=21
         ivt(178)=24
         ivt(179)=27
         ivt(180)=14
         ivt(181)=13
         ivt(182)=16
         ivt(183)=14
         ivt(184)=36
         ivt(185)=32
         ivt(186)=18
         ivt(187)=13
         ivt(188)=29
         ivt(189)=17
         ivt(190)=14
         ivt(191)=13
         ivt(192)=16
         ivt(193)=14
         ivt(194)=36
         ivt(195)=17
         ivt(196)=18
         ivt(197)=36
         ivt(198)=32
         ivt(199)=18
         ivt(200)=13
         ivt(201)=29
         ivt(202)=17
         ivt(203)=14
         ivt(204)=13
         ivt(205)=16
         ivt(206)=14
         ivt(207)=36
         ivt(208)=15
         ivt(209)=24
         ivt(210)=23
         ivt(211)=29
         ivt(212)=36
         ivt(213)=28
         ivt(214)=18
         ivt(215)=35
         ivt(216)=14
         ivt(217)=14
         ivt(218)=13
         ivt(219)=16
         ivt(220)=14
         ivt(221)=36
         ivt(222)=21
         ivt(223)=14
         ivt(224)=23
         ivt(225)=16
         ivt(226)=29
         ivt(227)=17
         ivt(228)=14
         ivt(229)=13
         ivt(230)=16
         ivt(231)=14
         ivt(232)=36
         ivt(233)=12
         ivt(234)=24
         ivt(235)=28
         ivt(236)=29
         ivt(237)=14
         ivt(238)=13
         ivt(239)=16
         ivt(240)=14
         ivt(241)=36
         ivt(242)=22
         ivt(243)=18
         ivt(244)=23
         ivt(245)=36
         ivt(246)=12
         ivt(247)=10
         ivt(248)=25
         ivt(249)=14
         ivt(250)=13
         ivt(251)=16
         ivt(252)=14
         ivt(253)=36
         ivt(254)=22
         ivt(255)=10
         ivt(256)=33
         ivt(257)=36
         ivt(258)=12
         ivt(259)=10
         ivt(260)=25
         ivt(261)=14
         ivt(262)=13
         ivt(263)=16
         ivt(264)=14
         ivt(265)=36
         ivt(266)=26
         ivt(267)=36
         ivt(268)=32
         ivt(269)=14
         ivt(270)=18
         ivt(271)=16
         ivt(272)=17
         ivt(273)=29
         ivt(274)=14
         ivt(275)=13
         ivt(276)=16
         ivt(277)=14
         ivt(278)=36
         ivt(279)=26
         ivt(280)=36
         ivt(281)=24
         ivt(282)=27
         ivt(283)=18
         ivt(284)=16
         ivt(285)=14
         ivt(286)=13
         ivt(287)=16
         ivt(288)=14
         ivt(289)=36
         ivt(290)=32
         ivt(291)=14
         ivt(292)=18
         ivt(293)=16
         ivt(294)=17
         ivt(295)=29
         ivt(296)=13
         ivt(297)=14
         ivt(298)=15
         ivt(299)=10
         ivt(300)=30
         ivt(301)=21
         ivt(302)=29
         ivt(303)=36
         ivt(304)=23
         ivt(305)=24
         ivt(306)=13
         ivt(307)=14
         ivt(308)=36
         ivt(309)=13
         ivt(310)=18
         ivt(311)=10
         ivt(312)=22
         ivt(313)=13
         ivt(314)=14
         ivt(315)=15
         ivt(316)=10
         ivt(317)=30
         ivt(318)=21
         ivt(319)=29
         ivt(320)=36
         ivt(321)=23
         ivt(322)=24
         ivt(323)=13
         ivt(324)=14
         ivt(325)=36
         ivt(326)=11
         ivt(327)=24
         ivt(328)=27
         ivt(329)=13
         ivt(330)=14
         ivt(331)=27
         ivt(332)=13
         ivt(333)=14
         ivt(334)=15
         ivt(335)=10
         ivt(336)=30
         ivt(337)=21
         ivt(338)=29
         ivt(339)=36
         ivt(340)=14
         ivt(341)=13
         ivt(342)=16
         ivt(343)=14
         ivt(344)=36
         ivt(345)=32
         ivt(346)=18
         ivt(347)=13
         ivt(348)=29
         ivt(349)=17
         ivt(350)=13
         ivt(351)=14
         ivt(352)=15
         ivt(353)=10
         ivt(354)=30
         ivt(355)=21
         ivt(356)=29
         ivt(357)=36
         ivt(358)=14
         ivt(359)=13
         ivt(360)=16
         ivt(361)=14
         ivt(362)=36
         ivt(363)=17
         ivt(364)=18
         ivt(365)=36
         ivt(366)=32
         ivt(367)=18
         ivt(368)=13
         ivt(369)=29
         ivt(370)=17
         ivt(371)=13
         ivt(372)=14
         ivt(373)=15
         ivt(374)=10
         ivt(375)=30
         ivt(376)=21
         ivt(377)=29
         ivt(378)=36
         ivt(379)=15
         ivt(380)=24
         ivt(381)=23
         ivt(382)=29
         ivt(383)=36
         ivt(384)=28
         ivt(385)=18
         ivt(386)=35
         ivt(387)=14
         ivt(388)=23
         ivt(389)=24
         ivt(390)=13
         ivt(391)=14
         ivt(392)=36
         ivt(393)=21
         ivt(394)=10
         ivt(395)=11
         ivt(396)=14
         ivt(397)=21
         ivt(398)=14
         ivt(399)=13
         ivt(400)=16
         ivt(401)=14
         ivt(402)=36
         ivt(403)=21
         ivt(404)=10
         ivt(405)=11
         ivt(406)=14
         ivt(407)=21
      endif
      return
      end

      subroutine intlstcat
c     catenate two or more lists
      include '../stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .lt. 1) then
         call error(39)
         return
      endif
      if (lhs .gt. 1) then
         call error(41)
         return
      endif
c
      if(rhs.eq.1) return
c
      lw=lstk(top+1)
      nel=0
      n1=0
      do 10 i=rhs-1,0,-1
         il=iadr(lstk(top-i))
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            nel=nel+1
            n1=n1+1
         else
            nel=nel+istk(il+1)
         endif
 10   continue
      if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) n1=n1+3
c     computes table of entry sizes istk(ilp+i),total size isz
      ilp=iadr(lw)+n1
      iln=ilp+nel
      err=sadr(iln)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ill=ilp-1
      isz=0
      do 12 i=rhs-1,0,-1
         il=iadr(lstk(top-i))
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            ill=ill+1
            istk(ill)=lstk(top-i+1)-lstk(top-i)
            isz=isz+istk(ill)
         else
            if(istk(il+1).gt.0) then
               do 11 j=1,istk(il+1)
                  ill=ill+1
                  istk(ill)=istk(il+2+j)-istk(il+1+j)
                  isz=isz+istk(ill)
 11            continue
            endif
         endif
 12   continue
c     copy entries
      ln=sadr(iln+3+nel)
      le=ln+isz
      err=le-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ill=ilp+nel
      do 20 i=1,rhs
         l=lstk(top+1-i)
         il=iadr(l)
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            lo=l
            ill=ill-1
            n=istk(ill)
         else
            ill=ill-istk(il+1)
            lo=sadr(il+3+istk(il+1))
            n=istk(il+2+istk(il+1))-1
         endif
         if(n.gt.0) then
            le=le-n
            call unsfdcopy(n,stk(lo),1,stk(le),1)
         endif
 20   continue
      top=top-rhs+1
      il=iadr(lstk(top))
      istk(il)=15
      istk(il+1)=nel
      istk(il+2)=1
      do 21 i=1,nel
         istk(il+2+i)=istk(il+1+i)+istk(ilp-1+i)
 21   continue
      l=sadr(il+3+nel)
      call unsfdcopy(isz,stk(ln),1,stk(l),1)
      lstk(top+1)=l+isz
      return
      end



      subroutine definedfields
c     returns index of defined fields of a list
      include '../stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c
      lw=lstk(top+1)
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).lt.15.or.istk(il).gt.17) then 
         err=1
         call error(44)
         return
      endif
      n=istk(il+1)

      err=lw+n-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      il=il+2
      l=lw
      do 10 i=1,n
         if(istk(il+1)-istk(il).gt.0) then
            stk(l)=i
            l=l+1
         endif
         il=il+1
 10   continue
      n=l-lw
      l=sadr(ilr+4)
      if(n.gt.0) then
         if(l.lt.lw) then
            call unsfdcopy(n,stk(lw),1,stk(l),1)
         else
            call unsfdcopy(n,stk(lw),-1,stk(l),-1)
         endif
      endif
      istk(ilr)=1
      istk(ilr+1)=min(1,n)
      istk(ilr+2)=n
      istk(ilr+3)=0
      lstk(top+1)=l+n
      return
      end


      subroutine intlstsize
c     WARNING : argument of this interface may be passed by reference
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).lt.15.or.istk(il).gt.17) then
         err=1
         call error(56)
         return
      endif
      n=istk(il+1)

      if(lhs*rhs.ne.1) then
         err=1
         call error(39)
         return
      endif
      ilr=iadr(lstk(top))
      l=sadr(ilr+4)
      err=l+1-lstk(bot)
      if (err.gt.0) then
         call error(17)
         return
      endif
      istk(ilr)=1
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      l=sadr(ilr+4)
      stk(l)=dble(n)
      lstk(top+1)=l+1
c
      return
      end
