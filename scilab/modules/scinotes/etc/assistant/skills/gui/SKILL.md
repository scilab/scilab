---
name: gui
description: |
  Building interactive interfaces in Scilab: uicontrol widgets and styles,
  callbacks, layouts (gridbag/border), dialogs, and the browser uicontrol
  (embedded HTML page with a JS <-> Scilab bridge, toScilab/fromScilab).
  Trigger words: "GUI", "uicontrol", "button", "slider", "callback",
  "dialog", "interface", "HTML page".
triggers: uicontrol, interface graphique, interface utilisateur, gui, ihm, bouton, boutons, button, buttons, slider, curseur, checkbox, case à cocher, cases à cocher, listbox, popupmenu, menu déroulant, liste déroulante, callback, boîte de dialogue, dialog, formulaire, form, widget, widgets, browser, html, javascript, js, css, page, panneau, panel, page html, html page, uigetfile, messagebox, app, application, tableau de bord, dashboard, fenêtre, window
---

# Scilab GUI (uicontrol)

## Figure and widgets

```scilab
f = figure("figure_name", "My tool", "dockable", "off", ...
           "infobar_visible", "off", "toolbar_visible", "off", ...
           "default_axes", "off", "layout", "gridbag");

b = uicontrol(f, "style", "pushbutton", "string", "Run", ...
              "callback", "onRun", "tag", "run_btn");
```

Styles: `text`, `edit`, `pushbutton`, `radiobutton`, `checkbox`, `slider`,
`listbox`, `popupmenu`, `frame`, `tab`, `layer`, `image`, `spinner`,
`browser` (embedded HTML, see below).

- Without a layout, place widgets with `"position", [x y w h]` (pixels,
  origin bottom-left).
- Read/write properties with dot notation: `b.string = "Stop";`
  values of inputs: `edit.string`, `slider.value`, `listbox.value`
  (index), `checkbox.value` (0/1), `popupmenu.value` (index into `.string`).
- Retrieve a widget by its tag with `h = get("run_btn")`, and set a
  property in one call with `set("run_btn", "string", "Stop")`. Do NOT
  write `findobj("tag", ...)` for this — it works but is slower and
  wordier; keep `findobj` for lookups on other properties.

## Callbacks

`"callback"` is either a Scilab instruction string or a function NAME:

```scilab
function onRun()
    ed = get("input");           // widget by tag
    disp("value: " + ed.string);
endfunction
```

The callback runs in the global context when the user acts. For sliders,
the callback fires on each move. Use `gcbo` inside a callback to get the
handle of the widget that triggered it.

## Layouts

```scilab
f = figure("layout", "gridbag");
c = createConstraints("gridbag", [1 1 1 1], [1 0], "horizontal");
uicontrol(f, "style", "edit", "constraints", c);
// border layout: createConstraints("border", "north") etc.
// frames can hold their own layout to nest structures
fr = uicontrol(f, "style", "frame", "layout", "border", "constraints", c2);
```

## Dialogs (no uicontrol needed)

`messagebox(msg)`, `x_dialog`, `x_choose(list, title)`, `x_mdialog`
(multi-field form), `uigetfile()/uiputfile()`, `uigetdir()`,
`uigetcolor()`, `input()`. Prefer these for one-shot questions.

## Browser uicontrol (embedded HTML + JS bridge)

A full HTML/JS page rendered inside the figure — for rich UIs
(canvas, CSS, charts) with logic staying in Scilab:

```scilab
uicontrol(f, "style", "browser", ...
          "string", SCI + "/mytool/page.html", ...   // file path or URL
          "callback", "onBrowserMsg", "tag", "web");
```

Page side (ScilabBrowser.js is available in the page):

```js
// JS -> Scilab: sends any JSON-able object; optional answer callback
toScilab({type: "update", data: getValues()});
toScilab({type: "compute", data: x}, (res) => show(res));

// Scilab -> JS pushes land here if the page defines it
function fromScilab(msg) { render(msg); }
```

Scilab pushes to the page by writing the browser's `data` property:

```scilab
set("web", "data", struct("type", "init", "data", struct("freq", 1)));
// received as fromScilab({type: "init", data: {freq: 1}}) in the page
```

Scilab side — the callback receives the message and a reply function:

```scilab
function onBrowserMsg(msg, cb)
    if type(msg) == 10 then          // first call: the STRING "loaded"
        return;                      // (page ready) — msg.type would fail
    end
    select msg.type
    case "update" then
        refreshPlot(msg.data);
    case "compute" then
        cb(struct("y", myCalc(msg.data)));   // answers the JS callback
    end
endfunction
```

`msg` arrives as a struct mirroring the JS object — except the very first
call, which is the string `"loaded"` once the page is ready. A common
pattern: the page sends `{type: "init"}` from its ready handler, and Scilab
answers by pushing the initial state through `data`.

`toScilab`/`fromScilab` come from `SCI/modules/gui/etc/ScilabBrowser.js`,
injected automatically into every page: do not include it yourself.

Checking a GUI: `get_figure` with `mode: "screen"` takes a screenshot of
the window as displayed — uicontrols, menus and the HTML page included (the
default export mode only renders the axes). Use it after each change to the
interface. The assistant cannot click in the page, though: exercise the
Scilab side by calling the callback directly with the struct the page would
send, e.g.
`onBrowserMsg(struct("type", "update", "data", struct("region", "Nord")), fakecb)`
with `function fakecb(r), disp(r), endfunction` standing in for the reply
function, then check the result with `get_figure` (`mode: "screen"`). To
test the page reacting to a push, set the browser `data` from Scilab and
take a screen capture. Keep the JS thin (`"debug", "on"` on the browser
uicontrol enables the dev tools for the user).

**Before writing a browser-based UI, ALWAYS read one of the reference
demos with your Read tool and mirror its structure** — this API is recent
and not in your training data:

- `SCI/modules/gui/demos/uicontrol_plot3dHTML.dem.sci` with its page
  `uicontrol_plot3d.html` + `js/uicontrol_plot3d.js` — the simplest complete
  Scilab/HTML/JS triplet (HTML controls driving a Scilab plot);
- `SCI/modules/gui/demos/uicontrol_dice_game_html.dem.sce` — round trips
  where Scilab answers the page through `cb(...)`;
- `SCI/modules/gui/demos/uicontrol_astar.dem.sce` — canvas-heavy page;
- `SCI/modules/statistics/demos/clustering/` — a real multi-panel app
  (several html/js/css files, one Scilab controller).

Documentation: `scilab_help("browser")` (the browser style and its
properties) and `scilab_help("uicontrol")`.

To turn `SCI/...` into a readable absolute path, run
`execute_scilab_code` with `mprintf("%s\n", SCI)` once.

`scilab:` links inside the page (e.g. `<a href="scilab:plot(1:10)">`)
echo the command at the console prompt before running it.

## Pitfalls

- JSON bridge shapes: a 1-element Scilab vector reaches JS as a scalar,
  not an array — normalize in JS (`Array.isArray(v) ? v : [v]`). JS arrays
  may reach Scilab as a `list` (type 15): convert before using them as a
  string vector.
- Axes inside a `frame` (the usual browser-app layout) are NOT rendered by
  `xs2png`/`xs2pdf` on the window, nor by `get_figure` in export mode: the
  image is blank. Use `get_figure` with `mode: "screen"` to check, and to
  export the plot copy the axes into a hidden plain figure first:
  `g = scf(1000); g.visible = "off"; a2 = copy(a, g); a2.axes_bounds = [0 0 1 1]; xs2png(g, file); delete(g);`
- An app must not `close(winsid())` at launch (it kills the user's other
  figures): close only its own window, found by tag
  (`h = findobj("tag", "my_app"); if h <> [] then delete(h); end`).
- To tell graphic objects apart use `h.type` ("Figure", "Axes", "Text",
  "uicontrol"...): `typeof(h)` is always "handle".

- Callbacks are strings evaluated later: build them with `msprintf` when
  they need values, or better, use function names + `get("tag")`/`gcbo`.
- A GUI figure should set `"default_axes", "off"` unless it also plots;
  plotting from a callback targets `gcf()` — save/restore the current
  figure (`scf`) to avoid drawing into the GUI window.
- Sizes in `"position"` are pixels and do not auto-resize: prefer layouts
  (gridbag/border) for anything more than a quick tool.
- Widget updates happen immediately, but long computations inside a
  callback freeze the interface: keep callbacks short.
