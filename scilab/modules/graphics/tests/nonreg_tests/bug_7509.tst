// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 7509 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7509
//
// <-- Short Description -->
// the legend help page does not correspond any more with the function (pos argument)
//
t=linspace(0,%pi,20);
drawlater()
clf();plot2d(t,[cos(t'),cos(2*t'),cos(3*t')],[-5,2 3]);  
ax=gca();
C=ax.children.children;

hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)']);
if hl.legend_location <> "in_upper_right" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],2);
if hl.legend_location <> "in_upper_left" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],"in_lower_right");
if hl.legend_location <> "in_lower_right" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],[2 0]);
if hl.legend_location <> "by_coordinates" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],%f);
if hl.legend_location <> "in_upper_right" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],2,%f);
if hl.legend_location <> "in_upper_left" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],"in_lower_right",%f);
if hl.legend_location <> "in_lower_right" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


delete(hl)
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],[2 0],%f);
if hl.legend_location <> "by_coordinates" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end




hl=legend('cos(t)','cos(2*t)','cos(3*t)');
if hl.legend_location <> "in_upper_right" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend('cos(t)','cos(2*t)','cos(3*t)',2,%f);
if hl.legend_location <> "in_upper_left" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


delete(hl)
hl=legend('cos(t)','cos(2*t)','cos(3*t)',"in_lower_right",%f);
if hl.legend_location <> "in_lower_right" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


delete(hl)
hl=legend('cos(t)','cos(2*t)','cos(3*t)',[2 0],%f);
if hl.legend_location <> "by_coordinates" then pause,end
if hl.line_mode <> "off" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end


hl=legend(C(1),C(2),C(3),['cos(t)';'cos(2*t)';'cos(3*t)']);
if hl.legend_location <> "in_upper_right" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(C(1),C(2),C(3),'cos(t)','cos(2*t)','cos(3*t)',2);
if hl.legend_location <> "in_upper_left" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(C,['cos(t)';'cos(2*t)';'cos(3*t)'],"in_lower_right");
if hl.legend_location <> "in_lower_right" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

delete(hl)
hl=legend(C,'cos(t)','cos(2*t)','cos(3*t)',[2 0]);
if hl.legend_location <> "by_coordinates" then pause,end
if or(hl.text<>["cos(t)";"cos(2*t)";"cos(3*t)"]) then pause,end

