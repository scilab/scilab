// Benchmark: scilab:image runtime generation
// Reproduces exactly what ScilabImageConverter does
// Run this in Scilab to measure per-image generation time

function _bench_image(name, code)
    imgFile = fullfile(TMPDIR, name + ".png");

    tic();

    __olddrv__ = driver();
    driver("png");
    xinit(imgFile);
    scf();
    rand("seed", 0);

    execstr(code);

    gcf().anti_aliasing = "2x";
    xend();
    driver(__olddrv__);

    elapsed = toc();
    mprintf("  %-30s : %.0f ms  (%s)\n", name, elapsed * 1000, imgFile);
endfunction

mprintf("\n=== Benchmark scilab:image generation ===\n\n");

// --- Test 1: Simple plot (très courant) ---
_bench_image("simple_plot", "plot(1:100, sin(1:100))");

// --- Test 2: Contourf (moyen) ---
_bench_image("contourf", "contourf(1:10, 1:10, rand(10,10), 5, 1:5, ""011"", "" "", [0,0,11,11])");

// --- Test 3: Plot avec styling (courant dans la doc) ---
code3 = [
    "x = linspace(0, 2*%pi, 200);"
    "plot(x, sin(x), ""r-"", x, cos(x), ""b--"");"
    "title(""Sine and Cosine"");"
    "xlabel(""x"");"
    "ylabel(""y"");"
    "legend(""sin"", ""cos"");"
];
_bench_image("styled_plot", strcat(code3, ascii(10)));

// --- Test 4: Champ (vecteurs) ---
_bench_image("champ", "champ(-5:5, -5:5, rand(11,11), rand(11,11))");

// --- Test 5: Surf 3D ---
code5 = [
    "[x, y] = meshgrid(-2:0.1:2, -2:0.1:2);"
    "z = x .* exp(-x.^2 - y.^2);"
    "surf(x, y, z);"
];
_bench_image("surf_3d", strcat(code5, ascii(10)));

// --- Test 6: Complexe (peaks-like) ---
code6 = [
    "x = linspace(-3, 3, 100);"
    "y = linspace(-3, 3, 100);"
    "[X, Y] = meshgrid(x, y);"
    "Z = 3*(1-X).^2 .* exp(-(X.^2) - (Y+1).^2) - 10*(X/5 - X.^3 - Y.^5) .* exp(-X.^2 - Y.^2) - 1/3*exp(-(X+1).^2 - Y.^2);"
    "contourf(x, y, Z, 20);"
    "//colorbar();"
];
_bench_image("peaks_contourf", strcat(code6, ascii(10)));

// --- Test 7: Histogramme ---
_bench_image("histogram", "histplot(20, rand(1, 10000, ""normal""))");

// --- Test 8: Pie chart ---
_bench_image("pie", "pie([15 25 35 25], [""A"" ""B"" ""C"" ""D""])");

// --- Résumé ---
mprintf("\n=== Benchmark terminé ===\n");
mprintf("Lancer dans Scilab : exec(""bench_image_gen.sce"", -1)\n");
