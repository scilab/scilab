// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_fulltest()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("fulltest.dem.sce");

    // DEMO START
    my_handle.figure_name="LaTeX Math";
    //grosse chaine latex
    square(0,0,15,15);
    xtitle("$\scalebox{1.6}\JLaTeXMath$");
    str="$\newcommand{\oa}{\left|}\newcommand{\fa}{\right|}\scalebox{1.6}{\begin{split}"+...
    "|I_2| &= \oa\int_0^T\psi(t)\left\{ u(a,t)-\int_{\gamma(t)}^a \frac{d\theta}{k} (\theta,t) \int_a^\theta c(\xi) u_t (\xi,t)\,d\xi\right\}dt\fa\\&\\&\mathrm{Produced\ with\ Java\ and\ \textcolor{OliveGreen}{\LaTeX}\ by\ }\mathscr{C}\mathcal{A}\mathfrak{L}\mathbf{I}\mathtt{X}\mathbb{T}\mathsf{E}.\\&\mbox{It handles accents (éàçèêïö\oe\ae) or cyrillic (привет мир)}\\&\mbox{or greek (γειά κόσμο) for our international users.}\\&\begin{pmatrix}\alpha&\beta&\gamma\\\aleph&\beth&\gimel\\\mathfrak{A}&\mathfrak{B}&\mathfrak{C}\\\end{pmatrix}\quad{(a+b)}^{\frac{n}{2}}=\sqrt{\sum_{k=0}^n\tbinom{n}{k}a^kb^{n-k}}\quad\\&\forall\varepsilon\in\mathbb{R}_+^*\ \exists\eta>0\ |x-x_0|\leq\eta\Longrightarrow|f(x)-f(x_0)|\leq\varepsilon\\&\det\begin{bmatrix}a_{11}&a_{12}&\cdots&a_{1n}\\a_{21}&\ddots&&\vdots\\\vdots&&\ddots&\vdots\\a_{n1}&\cdots&\cdots&a_{nn}\end{bmatrix}\overset{\mathrm{def}}{=}\sum_{\sigma\in\mathfrak{S}_n}\varepsilon(\sigma)\prod_{k=1}^n a_{k\sigma(k)}\\&\Delta f(x,y)=\frac{\partial^2f}{\partial x^2}+\frac{\partial^2f}{\partial y^2}\qquad\qquad \boxed{n!\underset{n\rightarrow+\infty}{\sim} {\left(\frac{n}{e}\right)}^n\sqrt{2\pi n}}\\&\fcolorbox{black}{Bittersweet}{\sideset{_\alpha^\beta}{_\gamma^\delta}{\begin{pmatrix}a&b\\c&d\end{pmatrix}}}\xrightarrow[T]{n\pm i-j}\sideset{^t}{}A\xleftarrow{\overrightarrow{u}\wedge\overrightarrow{v}}\underleftarrow{\iint_{\mathds{R}^2}e^{-\left(x^2+y^2\right)}\,\mathrm{d}x\mathrm{d}y}\end{split}}$";
    xstring(0,0,str);

    // DEMO END
endfunction

demo_fulltest();
clear demo_fulltest;