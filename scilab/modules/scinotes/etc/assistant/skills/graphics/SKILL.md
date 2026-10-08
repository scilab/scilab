---
name: graphics
description: |
  Scilab graphics and data visualization: figure/axes/entity object model, 2D and
  3D plots, property access (dot notation), colors and colormaps, figure export,
  annotations. Trigger words: "plot", "figure", "graph", "curve", "surface",
  "axes", "legend", "colormap", "subplot", "export figure", "visualize".
triggers: plot, plots, curve, curves, courbe, courbes, trace, tracer, tracé, traces, graphique, graphiques, figure, figures, graph, surface, axes, axe, legend, légende, colormap, subplot, histogram, histogramme, scatter, dessine, dessiner, visualize, visualise, camembert, pie chart, bar chart, diagramme, nuage de points, 2d, 3d, contour, isovaleurs, polaire, polar, barres, bar, png, pdf, svg, échelle log, log scale, colorbar, animation, anime
---

# Scilab graphics

## Graphics architecture
Scilab uses a hierarchical object model for graphics:
- **Figure** (top level): `gcf()` — contains axes, menus, etc.
- **Axes** (subplot): `gca()` — contains curves, surfaces, etc.
- **Entities** (curves, surfaces, etc.): `gce()` — last created entity

## Creating plots
```scilab
// 2D plots
plot(x, y)                    // basic line plot
plot(x, y, "r-o")            // red line with circle markers
plot2d(x, y, style=2)        // alternative 2D plot
bar(x, y)                    // bar chart
histplot(n, data)            // histogram
pie(x)                       // pie chart
polarplot(theta, rho)        // polar plot

// 3D plots
plot3d(x, y, z)              // 3D surface
mesh(x, y, z)                // wireframe
surf(x, y, z)                // filled surface
contour(x, y, z, n)          // contour plot
param3d(x, y, z)             // 3D parametric curve

// Figure management
scf()                        // new figure
scf(n)                       // figure number n
clf()                        // clear current figure
close()                      // close current figure
close(winsid())              // close ALL figures (xdel() was removed)
subplot(m, n, p)             // subplot
title("T"); xlabel("x"); ylabel("y"); xgrid();   // shortcut functions
```

Successive `plot` calls ADD to the current axes (there is no `hold on`);
call `clf()` to start over.

`plot` also accepts MATLAB-style properties:
`plot(x, y, "LineWidth", 2, "Color", "red", "Marker", "o", "MarkerSize", 4)`.

| MATLAB (does NOT exist) | Scilab |
|-------------------------|--------|
| `hold on/off` | nothing: plots accumulate; `clf()` to reset |
| `grid on` | `xgrid()` or `gca().grid = [1 1]` |
| `axis([x1 x2 y1 y2])`, `xlim`, `ylim` | `gca().data_bounds = [x1 y1; x2 y2]` |
| `axis equal` | `gca().isoview = "on"` |
| `text(x, y, s)` | `xstring(x, y, s)` |
| `stairs` / `stem` | `plot2d2` / `plot2d3` |
| `plot3` | `param3d` |
| `scatter3` | `scatter3d` |
| `histogram` | `histplot(nbins, data)` |
| `fill` / `area` | `xfpoly` |
| `boxplot`, `datetick`, `annotation` | none — build by hand |
| `viridis` | `parula` (also jet, hot, cool, gray, hsv, rainbow...) |

## Time axis (datetime)
`plot` does NOT accept `datetime` x values. Convert to seconds and label
the ticks yourself (what `stackedplot` does internally):
```scilab
x = d.date * 86400 + d.time;                    // d: datetime vector
plot(x, y);
loc = linspace(x(1), x($), 7)';
lab = datetime(zeros(loc), 1, 1, "OutputFormat", "HH:mm");
lab.date = floor(loc / 86400);  lab.time = modulo(loc, 86400);
gca().x_ticks = tlist(["ticks", "locations", "labels"], loc, string(lab));
```
For a timeseries, `stackedplot(ts)` does this automatically (one axes per
variable, creates its own figure).

## Property access
Two ways to access/modify properties:
```scilab
// Method 1: dot notation (preferred)
a = gca();
a.font_size = 3;
a.x_label.text = "Time (s)";
a.y_label.text = "Amplitude";
a.title.text = "My Plot";

// Method 2: set/get functions
set(gca(), "font_size", 3);
val = get(gca(), "font_size");
```

## Common figure properties
```scilab
f = gcf();
f.figure_size = [800, 600];       // window size in pixels
f.background = color("white");     // background color
f.figure_name = "My Figure";       // window title
```

## Common axes properties
```scilab
a = gca();
// Labels and title
a.title.text = "Title";
a.x_label.text = "X";
a.y_label.text = "Y";
a.title.font_size = 4;

// Axis limits
a.data_bounds = [xmin, ymin; xmax, ymax];
a.tight_limits = "on";
a.isoview = "on";

// Grid
a.grid = [1, 1];                   // grid on both axes
a.grid_style = [8, 8];            // dashed grid

// Log scale
a.log_flags = "lnn";              // x=log, y=normal, z=normal
// in log mode data_bounds are REAL values, strictly positive (not log10)

// Legend
legend(["curve1"; "curve2"]);
hl = gce();                        // legend entity
hl.font_size = 2;
```

## Two y axes (no yyaxis)
```scilab
plot(x, y1);                      // left axis
a2 = newaxes();                   // second axes over the first
a2.y_location = "right";  a2.filled = "off";  a2.axes_visible(1) = "off";
plot(x, y2, "r");                 // drawn in a2, scaled on the right
```

## Common curve (polyline) properties
```scilab
plot(x, y);
e = gce();                         // get the polyline entity
e.children(1).foreground = color("red");
e.children(1).line_style = 2;     // dashed
e.children(1).thickness = 2;
e.children(1).mark_mode = "on";
e.children(1).mark_style = 9;     // circle
e.children(1).mark_size = 8;
```

## Color management
```scilab
color("red")                       // predefined color name
color(255, 0, 0)                   // RGB values (0-255)
// Color maps
f = gcf();
f.color_map = jet(64);             // jet colormap
f.color_map = hot(64);             // hot colormap
f.color_map = cool(64);            // cool colormap
f.color_map = gray(64);            // grayscale colormap
// NOTE: jetcolormap(), hotcolormap() etc. were REMOVED — use jet(), hot() etc.
```

## Export figures
```scilab
xs2png(gcf(), "figure.png");       // export to PNG
xs2pdf(gcf(), "figure.pdf");       // export to PDF
xs2svg(gcf(), "figure.svg");       // export to SVG
xs2eps(gcf(), "figure.eps");       // export to EPS
```

## Animation
Draw once, then only update `.data` in the loop. The `pixmap` property and
`show_pixmap()` NO LONGER exist.
```scilab
plot(1, 0, "ro");  pt = gce().children(1);
for k = 1:360
    drawlater();                 // or f.immediate_drawing = "off"
    pt.data = [cosd(k) sind(k)];
    drawnow();
    sleep(20);                   // ms
end
```

## Interaction (clicks)
- Show coordinates on click: `datatipManagerMode(gcf(), "on")` (built-in
  datatips) or `datatipCreate(polyline, index)` programmatically
- Custom reaction: `seteventhandler("my_handler")` with
  `function my_handler(win, x, y, ibut)`, `xchange(x, y, "i2f")` for pixels ->
  axes coordinates; `seteventhandler("")` to stop
- `xclick()` / `xgetmouse()` BLOCK until a click: never call them from the
  assistant (the tool call would hang)

## Annotations
```scilab
xstring(x, y, "text")             // place text at position
xarrows([x1, x2], [y1, y2])      // draw arrow
xrect(x, y, w, h)                // draw rectangle
xarc(x, y, w, h, a1, a2)         // draw arc
```

## Guidelines
- Always use `get_figure` after creating a plot to verify the result; pass
  `figure_id` when several figures are open (functions like `stackedplot`
  create their own figure — an empty `scf(0)` done before would be captured)
- Prefer dot notation for property access (more readable)
- Use `clf()` before creating a new plot to avoid overlapping
- Use `scf()` to create a new figure window if needed
- Remember: Scilab uses underscores in property names (font_size, not fontSize)
- LaTeX in titles/labels only works when the WHOLE string is enclosed in
  `$...$`: `"Angle $\theta$ (rad)"` is shown literally; write
  `"$\text{Angle } \theta \text{ (rad)}$"` (MathML `<...>` also supported)
- To test what a graphic object is, use `h.type` ("Axes", "Polyline",
  "Text"...): `typeof(h)` is always "handle"
