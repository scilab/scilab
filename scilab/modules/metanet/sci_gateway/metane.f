c  interface function 
c   ********************
       subroutine gw_metanet
       include '../../core/includes/stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
     $ ,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,4
     $ 5,46,47,48,49,50) fin
       return
1      call intsm6loadg('m6loadg')
       return
2      call intsm6saveg('m6saveg')
       return
3      call intsm6prevn2p('m6prevn2p')
       return
4      call intsm6ns2p('m6ns2p')
       return
5      call intsm6p2ns('m6p2ns')
       return
6      call intsm6edge2st('m6edge2st')
       return
7      call intsm6prevn2st('m6prevn2st')
       return
8      call intsm6compc('m6compc')
       return
9      call intsm6concom('m6concom')
       return
10      call intsm6compfc('m6compfc')
       return
11      call intsm6sconcom('m6sconcom')
       return
12      call intsm6pcchna('m6pcchna')
       return
13      call intsm6fordfulk('m6fordfulk')
       return
14      call intsm6johns('m6johns')
       return
15      call intsm6dijkst('m6dijkst')
       return
16      call intsm6frang('m6frang')
       return
17      call intsm6chcm('m6chcm')
       return
18      call intsm6transc('m6transc')
       return
19      call intsm6dfs('m6dfs')
       return
20      call intsm6umtree('m6umtree')
       return
21      call intsm6umtree1('m6umtree1')
       return
22      call intsm6dmtree('m6dmtree')
       return
23      call intsm6tconex('m6tconex')
       return
24      call intsm6flomax('m6flomax')
       return
25      call intsm6kilter('m6kilter')
       return
26      call intsm6busack('m6busack')
       return
27      call intsm6floqua('m6floqua')
       return
28      call intsm6relax('m6relax')
       return
29      call intsm6findiso('m6findiso')
       return
30      call intsm6ta2lpd('m6ta2lpd')
       return
31      call intsm6ta2lpu('m6ta2lpu')
       return
32      call intsm6lp2tad('m6lp2tad')
       return
33      call intsm6lp2tau('m6lp2tau')
       return
34      call intsm6dfs2('m6dfs2')
       return
35      call intsm6diam('m6diam')
       return
36      call intsm6cent('m6cent')
       return
37      call intsm6hullcvex('m6hullcvex')
       return
38      call intsm6clique('m6clique')
       return
39      call intsm6clique1('m6clique1')
       return
40      call intsm6hamil('m6hamil')
       return
41      call intsm6visitor('m6visitor')
       return
42      call intsm6bmatch('m6bmatch')
       return
43      call intsm6knapsk('m6knapsk')
       return
44      call intsm6prfmatch('m6prfmatch')
       return
45      call intsm6permuto('m6permuto')
       return
46      call intsm6mesh2b('m6mesh2b')
       return
47      call intsm6deumesh('m6deumesh')
       return
48      call intsm6bandred('m6bandred')
       return
49      call intsm6meshmesh('m6meshmesh')
       return
50      call intsm6ford('m6ford')
       return
       end
