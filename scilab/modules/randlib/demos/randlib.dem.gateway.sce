// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("randlib.dem.gateway.sce");
    add_demo(_("Random"), demopath + "randlib.dem.gateway.sce");

    subdemolist = [_("binomial random variable")        ,"binomial.dem.sce"      ; ..
    _("discrete random variable")        ,"discrete.dem.sce"      ; ..
    _("discrete uniform random variable"),"discrete_uni.dem.sce"  ; ..
    _("geometric random variable")       ,"geometric.dem.sce"     ; ..
    _("Poisson random variable")         ,"poisson.dem.sce"       ; ..
    _("Exponential random variable")     ,"exp.dem.sce"           ; ..
    _("Weibull random variable")         ,"weibull.dem.sce"       ; ..
    _("Hyper geometric random variable") ,"hypergeom.dem.sce"     ; ..
    _("Erlang random variable")          ,"erlang.dem.sce"        ];

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();
clear demo_gateway;
