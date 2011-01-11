// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function latex_callback()
messagebox("LaTeX is beautiful !", "", "info", "modal");
endfunction

function mathml_callback()
messagebox("MathML is beautiful !", "", "info", "modal");
endfunction

// Create a figure
f = figure("figure_name", gettext("Uicontrols demo with LaTeX"),...
    "Position",[50 50 300 195],...
    "BackgroundColor",[0.9 0.9 0.9],...
    "Tag", "uicontrols_LaTeX_demo_figure");

// Remove Scilab 'standard' menus
delmenu(f.figure_id, gettext("&File"));
delmenu(f.figure_id, gettext("&Tools"));
delmenu(f.figure_id, gettext("&Edit"));
delmenu(f.figure_id, gettext("&?"));

// Disable graphics toolbar
toolbar(f.figure_id, "off");

// Adjust figure size after toolbar removing
f.position = [50 50 330 160];

// Add custom menus
mlatex=uimenu(f,'label', '$\LaTeX$');
ml1=uimenu(mlatex,'label', '$\int_0^\infty\mathrm{e}^{-x^2}\,dx$', 'Callback', 'latex_callback()');
ml2=uimenu(mlatex,'label', '$\frac\sqrt{\pi}2$', 'Callback', 'latex_callback()');

mmathml=uimenu(f,'label', 'MathML');
mm1=uimenu(mmathml,'label', '<msup><mn>x</mn><mi>2</mi></msup>', 'Callback', 'mathml_callback()');
mm2=uimenu(mmathml, 'label', '<mrow><msup><mn>a</mn><mi>2</mi></msup><mo>+</mo><msup><mn>b</mn><mi>2</mi></msup><mo>=</mo><msup><mn>c</mn><mi>2</mi></msup></mrow>', ..
		    'Callback', 'mathml_callback()');

latexButton=uicontrol(f,'style','pushbutton','string','$\scalebox{2}{\LaTeX\ x^2}$', 'Callback', 'latex_callback()');
latexButton.Position = [30, 70, 120, 40];
latexLabel=uicontrol(f,'Style','text','string',"$\text{Text: }\Gamma(s)=\int_0^\infty t^{s-1}\mathrm{e}^{-t}\,\mathrm{d}t$");
latexLabel.fontsize=15;
latexLabel.Position = latexLabel.Position + [50, 0, 0, 0];
mathmlButton=uicontrol(f, 'style', 'pushbutton', 'string', '<mrow><mtext>MathML</mtext><mphantom><mi>a</mi></mphantom><msup><mi>x</mi><mn>2</mn></msup></mrow>',  ..
			  'Callback', 'mathml_callback()');
mathmlButton.Position = latexButton.Position + [140, 0, 0, 0];
