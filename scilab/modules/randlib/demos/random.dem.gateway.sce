//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("random.dem.gateway.sce");

subdemolist = ["binomial random variable"        ,"binomial.dem.sce"      ; ..
               "discrete random variable"        ,"discrete.dem.sce"      ; ..
               "discrete uniform random variable","discrete_uni.dem.sce"  ; ..
               "geometric random variable"       ,"geometric.dem.sce"     ; ..
               "Poisson random variable"         ,"poisson.dem.sce"       ; ..
               "Exponential random variable"     ,"exp.dem.sce"           ; ..
               "Weibull random variable"         ,"weibull.dem.sce"       ; ..
               "Hyper geometric random variable" ,"hypergeom.dem.sce"     ; ..
               "Erlang random variable"          ,"erlang.dem.sce"        ];

subdemolist(:,2) = demopath + subdemolist(:,2);
