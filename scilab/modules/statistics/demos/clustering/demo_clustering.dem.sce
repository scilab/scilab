// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function demo_clustering()
    close(100002);

    fig = figure(...
        "figure_id", 100002, ...
        "figure_position", [50 50], ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "dockable", "off", ...
        "menubar", "none", ...
        "default_axes", "off", ...
        "axes_size", [1300 800], ...
        "layout", "border", ...
        "background", -2, ...
        "visible", "off", ...
        "figure_name", "Clustering — Interactive Demo");

    // left frame - clustering choice
    fr_left = uicontrol(fig, ...
        "style", "frame", ...
        "layout", "border", ...
        "backgroundcolor", [1 1 1], ...
        "constraints", createConstraints("border", "left", [280, 0]));

    uicontrol(fr_left, ...
        "style", "browser", ...
        "string", fullfile(SCI,"modules","statistics","demos", "clustering", "clustering_panel.html"), ...
        "callback", "cbClustering", ...
        "tag", "clustering_browser");

    // right frame - legend
    fr_right = uicontrol(fig, ...
        "style", "frame", ...
        "layout", "border", ...
        "backgroundcolor", [1 1 1], ...
        "constraints", createConstraints("border", "right", [220, 0]));

    results_fr = uicontrol(fr_right, ...
        "style", "browser", ...
        "string", fullfile(SCI,"modules","statistics","demos", "clustering", "results_panel.html"), ...
        "tag", "results_browser");

    [colorsHex, colorHexNoise] = colorHEXList(fig);
    results_fr.userdata = struct("colors_hex", colorsHex, "colors_noise_hex", colorHexNoise);

    // center - draw 
    fr_center = uicontrol(fig, ...
        "style", "frame", ...
        "backgroundcolor", [1 1 1], ...
        "tag", "fr_center", ...
        "constraints", createConstraints("border", "center"));

    // raw data
    grand("setsd", 42);
    c1 = [grand(50, 1, "nor", 1.0, 0.25), grand(50, 1, "nor", 1.0, 0.25)];
    c2 = [grand(50, 1, "nor", 4.0, 0.25), grand(50, 1, "nor", 1.0, 0.25)];
    c3 = [grand(50, 1, "nor", 2.5, 0.25), grand(50, 1, "nor", 4.0, 0.25)];
    ns = [grand(20, 1, "unf", 0.0, 5.5),  grand(20, 1, "unf", 0.0, 5.5)];

    data = [c1; c2; c3; ns];
    fr_center.userdata = data;
    x = data(:, 1);
    y = data(:, 2);
    mg = 0.5;
    bounds = [min(x) - mg, min(y) - mg; ...
              max(x) + mg, max(y) + mg];

    // graphs - subplot
    ax = newaxes(fr_center);

    // subplot 1 - raw data
    ax1 = subplot(2, 2, 1)
    ax1.tag = "raw_data_plot";
    p = plot2d(data(:, 1), data(:, 2), style=-4);
    p.mark_size = 7;
    p.mark_foreground = color("steelblue4");
    ax1.data_bounds = bounds;
    ax1.title.text = "Raw data  (N = " + string(size(data, 1)) + ")";
    ax1.box = "on";
    ax1.isoview = "on";

    // subplot 2 - dbscan
    ax2 = subplot(2, 2, 2);
    ax2.box = "on";
    ax2.isoview = "on";
    ax2.tag = "dbscan_plot";

    // subplot 3 - meanshift
    ax3 = subplot(2, 2, 3);
    ax3.box = "on";
    ax3.isoview = "on";
    ax3.tag = "meanshift_plot";

    // subplot4 - kmeans
    ax4 = subplot(2, 2, 4);
    ax4.box = "on";
    ax4.isoview = "on";
    ax4.tag = "kmeans_plot";

    fig.visible = "on";

endfunction

function cbClustering(msg, cb)

    if msg == "loaded" then
        data = get("fr_center", "userdata");
        [db_cl, db_no, db_sz] = dbscan_plot(data, 0.5, 5, "euclidean");
        [ms_cl, ms_sz] = meanshift_plot(data, 1.3);
        [km_k, km_sz] = kmeans_plot(data, 3);
        set("results_browser", "data", all_struct(db_cl, db_no, db_sz, ms_cl, ms_sz, km_k, km_sz));
        return;
    end

    parts = strsplit(msg, "|");

    // regenerate data: "regen|N|eps|mpts|metric|bw|k"
    if parts(1) == "regen" then
        n_pts = round(strtod(parts(2)));
        eps = strtod(parts(3));
        mpts = round(strtod(parts(4)));
        metric = parts(5);
        bw = strtod(parts(6));
        k = round(strtod(parts(7)));
        data = regenerate_data(n_pts);
        [db_cl, db_no, db_sz] = dbscan_plot(data, eps, mpts, metric);
        [ms_cl, ms_sz] = meanshift_plot(data, bw);
        [km_k,  km_sz] = kmeans_plot(data, k);
        set("results_browser", "data", all_struct(db_cl, db_no, db_sz, ms_cl, ms_sz, km_k, km_sz));
        cb("");
        return;
    end

    data = get("fr_center", "userdata");

    // dbscan only: "dbscan|eps|mpts|metric"
    if parts(1) == "dbscan" then
        eps = strtod(parts(2));
        mpts = round(strtod(parts(3)));
        metric = parts(4);
        [db_cl, db_no, db_sz] = dbscan_plot(data, eps, mpts, metric);
        set("results_browser", "data", dbscan_struct(db_cl, db_no, db_sz));
        cb("");
        return;
    end

    // meanshift only: "meanshift|bw"
    if parts(1) == "meanshift" then
        bw = strtod(parts(2));
        [ms_cl, ms_sz] = meanshift_plot(data, bw);
        set("results_browser", "data", meanshift_struct(ms_cl, ms_sz));
        cb("");
        return;
    end

    // kmeans only: "kmeans|k"
    if parts(1) == "kmeans" then
        k = round(strtod(parts(2)));
        [km_k, km_sz] = kmeans_plot(data, k);
        set("results_browser", "data", kmeans_struct(km_k, km_sz));
        cb("");
        return;
    end

    if parts(1) == "help" then
        doc(parts(2));
        return
    end

endfunction

function colors = colorList()
    colors = [color("red"), color("royalblue4"), color("darkgreen"), ...
              color("darkorange3"), color("purple4"), color("deeppink3"), ...
              color("turquoise4"),  color("sienna"), color("slateblue3"), ...
              color("olivedrab4")];
endfunction

function hex = color2hex(fig_h, idx)
    cmap = fig_h.color_map;
    rgb  = cmap(idx, :);
    r = max(0, min(255, round(rgb(1) * 255)));
    g = max(0, min(255, round(rgb(2) * 255)));
    b = max(0, min(255, round(rgb(3) * 255)));
    hex = sprintf("#%02X%02X%02X", r, g, b);
endfunction

function [colorsHex, colorHexNoise] = colorHEXList(fig)
    colors = colorList();
    colorsHex = color2hex(fig, colors(1));
    for k = 2:length(colors)
        colorsHex = [colorsHex, color2hex(fig, colors(k))];
    end
    colorHexNoise = color2hex(fig, color("gray"));
endfunction

function data = regenerate_data(n_pts)

    n_cl = min(5, max(2, round(sqrt(n_pts / 20))));
    n_noise = max(5, round(n_pts * 0.12));
    n_per = floor((n_pts - n_noise) / n_cl);
    rem = (n_pts - n_noise) - n_cl * n_per;

    size_cl = n_per * ones(1, n_cl);
    if rem > 0 then
        size_cl(1:rem) = size_cl(1:rem) + 1;
    end

    all_cx = [1.0, 4.0, 2.5, 4.0, 1.0];
    all_cy = [1.0, 1.0, 4.0, 4.0, 3.0];

    data_pts = [];
    for k = 1:n_cl
        nk = size_cl(k);
        ck = [grand(nk,1,"nor",all_cx(k),0.25), ...
              grand(nk,1,"nor",all_cy(k),0.25)];
        data_pts = [data_pts; ck];
    end

    ns = [grand(n_noise,1,"unf",0.0,5.5), grand(n_noise,1,"unf",0.0,5.5)];

    data = [data_pts; ns];
    X = data(:,1);
    Y = data(:,2);
    mg = 0.5;
    bounds = [min(X)-mg, min(Y)-mg; max(X)+mg, max(Y)+mg];

    set("fr_center", "userdata", data);

    drawlater();
    ax1 = get("raw_data_plot");
    sca(ax1);
    if ~isempty(ax1.children) then 
        delete(ax1.children); 
    end
    p = plot2d(X, Y, style=-4);
    p.mark_size = 7;
    p.mark_foreground = color("steelblue4");
    ax1.data_bounds = bounds;
    ax1.title.text = "Raw data  (N = " + string(size(data,1)) + ")";
    drawnow();
endfunction

function [n_clusters, n_noise, cluster_sizes] = dbscan_plot(data, eps, mpts, metric)

    labels = dbscan(data, eps, mpts, metric);
    n_clusters = max(0, max(labels));
    noise_idx = find(labels == -1);
    n_noise = length(noise_idx);

    cluster_sizes = zeros(1, n_clusters);
    for k = 1:n_clusters
        cluster_sizes(k) = sum(labels == k);
    end

    drawlater();
    ax = get("dbscan_plot");
    sca(ax);
    if ~isempty(ax.children) then 
        delete(ax.children); 
    end

    if ~isempty(noise_idx) then
        p = plot2d(data(noise_idx, 1), data(noise_idx, 2), style=-4);
        p.mark_size = 6;
        p.mark_foreground = color("gray");
    end

    colors = colorList();

    for k = 1:n_clusters
        cidx = find(labels == k);
        // if ~isempty(cidx) then
            p = plot2d(data(cidx, 1), data(cidx, 2), style=-4);
            c = colors(modulo(k-1, length(colors)) + 1);
            p.mark_size = 8;
            p.mark_foreground = c;
        // end
    end

    ax.data_bounds = get("raw_data_plot", "data_bounds");
    ax.title.text  = "DBSCAN  (eps=" + string(round(eps*100)/100) + ..
                         "  minPts=" + string(mpts) + "  " + metric + ")";
    drawnow();
endfunction

function [n_clusters, cluster_sizes] = meanshift_plot(data, bw)

    [centers, labels] = meanshift(data, bw);
    n_clusters = max(0, max(labels));

    cluster_sizes = zeros(1, n_clusters);
    for k = 1:n_clusters
        cluster_sizes(k) = sum(labels == k);
    end

    drawlater();
    ax = get("meanshift_plot");
    sca(ax);
    if ~isempty(ax.children) then 
        delete(ax.children); 
    end

    colors = colorList();

    for k = 1:n_clusters
        cidx = find(labels == k);
        if ~isempty(cidx) then
            p = plot2d(data(cidx, 1), data(cidx, 2), style=-4);
            c = colors(modulo(k-1, length(colors)) + 1);
            p.mark_size = 8;
            p.mark_foreground = c;
        end
    end

    ax.data_bounds = get("raw_data_plot", "data_bounds");
    ax.title.text  = "Mean Shift  (bandwidth=" + string(round(bw*100)/100) + ")";
    drawnow();
endfunction

function [n_clusters, cluster_sizes] = kmeans_plot(data, k_val)

    [labels, centers] = kmeans(data, k_val);
    n_clusters = k_val;

    cluster_sizes = zeros(1, k_val);
    for i = 1:k_val
        cluster_sizes(i) = sum(labels == i);
    end

    drawlater();
    ax = get("kmeans_plot");
    sca(ax);
    if ~isempty(ax.children) then
        delete(ax.children);
    end

    colors = colorList();

    for i = 1:k_val
        cidx = find(labels == i);
        if ~isempty(cidx) then
            p = plot2d(data(cidx, 1), data(cidx, 2), style=-4);
            c = colors(modulo(i-1, length(colors)) + 1);
            p.mark_size = 8;
            p.mark_foreground = c;
        end
    end

    ax.data_bounds = get("raw_data_plot", "data_bounds");
    ax.title.text  = "K-Means  (k=" + string(k_val) + ")";
    drawnow();
endfunction

function s = dbscan_struct(db_cl, db_no, db_sz)
    st = get("results_browser", "userdata");
    colorsHex = st.colors_hex;
    colorHexNoise = st.colors_noise_hex;

    n_colors = size(colorsHex, 2);
    colors = colorsHex(modulo(0:(db_cl-1), n_colors) + 1);

    s = struct("type", "dbscan", ...
               "n_clusters", db_cl, ...
               "n_noise", db_no, ...
               "noise_hex", colorHexNoise, ...
               "counts", db_sz, ...
               "colors", colors);
endfunction

function s = meanshift_struct(ms_cl, ms_sz)
    st = get("results_browser", "userdata");
    colorsHex = st.colors_hex;

    n_colors = size(colorsHex, 2);
    colors = colorsHex(modulo(0:(ms_cl-1), n_colors) + 1);
    s = struct("type", "meanshift", ...
               "n_clusters", ms_cl, ...
               "counts", ms_sz, ...
               "colors", colors);
endfunction

function s = kmeans_struct(km_k, km_sz)
    st = get("results_browser", "userdata");
    colorsHex = st.colors_hex;
    n_colors = size(colorsHex, 2);
    colors = colorsHex(modulo(0:(km_k-1), n_colors) + 1);
    s = struct("type", "kmeans", ...
               "n_clusters", km_k, ...
               "counts", km_sz, ...
               "colors", colors);
endfunction

function s = all_struct(db_cl, db_no, db_sz, ms_cl, ms_sz, km_k, km_sz)
    s = struct("type", "all");
    s.dbscan = dbscan_struct(db_cl, db_no, db_sz);
    s.meanshift = meanshift_struct(ms_cl, ms_sz);
    s.kmeans = kmeans_struct(km_k, km_sz);
endfunction

demo_clustering();
