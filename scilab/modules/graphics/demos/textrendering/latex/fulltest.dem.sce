//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - Calixte DENIZET
//
// This file is distributed under the same license as the Scilab package.
//

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("fulltest.dem.sce");

// DEMO START
my_handle.figure_name="LaTeX Math";
//grosse chaine latex
square(0,0,10,10);
xtitle("$\mathbb{J}\LaTeX Math$");
str="$\newcommand{\oa}{\left|}\newcommand{\fa}{\right|}\begin{split}"+...
	" |I_2| &= \oa\int_0^T\psi(t)\left\{ u(a,t)-\int_{\gamma(t)}^a \frac{d\theta}{k} (\theta,t) \int_a^\theta c(\xi) u_t (\xi,t)\,d\xi\right\}dt\fa\\&\le C_6 \Bigg|\oa f \int_\Omega \oa\widetilde{S}^{-1,0}_{a,-} W_2(\Omega, \Gamma_1)\fa\ \fa\oa |u|\overset{\circ}{\to} W_2^{\widetilde{A}}(\Omega;\Gamma_r,T)\fa\Bigg|\\&\\&\mathrm{Produced\ with\ Java\ and\ \LaTeX\ by\ }\mathscr{C}\mathcal{A}\mathfrak{L}\mathbf{I}\mathtt{X}\mathbb{T}\mathsf{E}\\&\mathrm{and\ also\ handles\ accents\ like\ éàçèêïö\ for\ our\ international\ users}\\&\begin{pmatrix}\alpha&\beta&\gamma&\delta\\\aleph&\beth&\gimel&\daleth\\\mathfrak{A}&\mathfrak{B}&\mathfrak{C}&\mathfrak{D}\\\boldsymbol{\mathfrak{a}}&\boldsymbol{\mathfrak{b}}&\boldsymbol{\mathfrak{c}}&\boldsymbol{\mathfrak{d}}\end{pmatrix}\quad{(a+b)}^n=\sum_{k=0}^n\tbinom{n}{k}a^kb^{n-k}\quad \Biggl(\biggl(\Bigl(\bigl(()\bigr)\Bigr)\biggr)\Biggr)\\&\forall\varepsilon\in\mathbb{R}_+^*\ \exists\eta>0\ |x-x_0|\leq\eta\Longrightarrow|f(x)-f(x_0)|\leq\varepsilon\\&\det\begin{bmatrix}a_{11}&a_{12}&\cdots&a_{1n}\\a_{21}&\ddots&&\vdots\\\vdots&&\ddots&\vdots\\a_{n1}&\cdots&\cdots&a_{nn}\end{bmatrix}\overset{\mathrm{def}}{=}\sum_{\sigma\in\mathfrak{S}_n}\varepsilon(\sigma)\prod_{k=1}^n a_{k\sigma(k)}\\&\Delta f(x,y)=\frac{\partial^2f}{\partial x^2}+\frac{\partial^2f}{\partial y^2}\qquad\qquad \boxed{n!\underset{n\rightarrow+\infty}{\sim} {\left(\frac{n}{e}\right)}^n\sqrt{2\pi n}}\\&\sideset{_\alpha^\beta}{_\gamma^\delta}{\begin{pmatrix}a&b\\c&d\end{pmatrix}}\xrightarrow[T]{n\pm i-j}\sideset{^t}{}A\xleftarrow{\overrightarrow{u}\wedge\overrightarrow{v}}\underleftarrow{\iint_{\mathds{R}^2}e^{-\left(x^2+y^2\right)}\,\mathrm{d}x\mathrm{d}y}\end{split}$";
xstring(0,0,str);

// DEMO END
