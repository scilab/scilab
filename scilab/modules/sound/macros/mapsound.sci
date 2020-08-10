// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = mapsound(w, dt, frange, rate, Colormap)
    // rate: Sampling rate [Hz]
    // frange = fmax (with fmin=0) | [fmin fmax]
    // varargout = list(dates_vector, frequencies_vector, amplitudes_matrix)
    if ~isdef("rate","l") | rate==[] then
        rate = 22050
    end
    nSamples = max(size(w))
    T = nSamples / rate

    if ~isdef("frange","l") | frange==[] then
        fmin = 0
        fmax = -0.2     // Could be set according to an amplitude threshold
    elseif length(frange)==1
        fmin = 0
        fmax = frange
    else
        [fmin, fmax]= (frange(1), frange(2))
    end
    if fmin < 0
        fmin = rate * abs(fmin)
    end
    if fmax < 0 & abs(fmax) < 1
        fmax = rate * abs(fmax)
    end

    if ~isdef("dt","l") | dt==[] then
        nspect = floor(sqrt(fmax*T))    // => nspect =~= nfreq
        dt = T / nspect
    elseif dt < 0   // -dt indicates the wished number of spectra
        dt = abs(dt) * T
        nspect = floor(T / dt)
    else
        nspect = floor(T / dt)
    end
    if ~isdef("fmin","l") then
        fmin = 0
    end
    //
    dp = round(dt*rate)
    [x, y] = ndgrid(1:dp, 0:(nspect-1))
    ind = x + floor(y * dt * rate);

    M = abs(fft(w(ind), -1, 1))
    M = M(1:ceil(dp/2), :) // spectra are even, since the input signal is real

    df = 1 / (dp/rate);
    frequencies = (0:(dp-ceil(dp/2))) * df
    sel = find(frequencies >= fmin & frequencies <= fmax)
    M = M(sel,:) / length(frequencies)
    frequencies = frequencies(sel)
    dates = (0:size(M,2)-1) * dt

    // Display
    // -------
    // Set a smart colormap if it is not disturbing:
    gridColor = 0 // default value = black
    // menus are not clf reset and must not prevent initializing the colormap:
    if winsid() <> [] then
        tmp = gca().parent.children.type
        tmp(tmp=="uimenu") = []
    end
    if isdef("Colormap","l") | winsid()==[] | (size(tmp,"*")==1 & ..
        size(gcf().color_map,1)==size(gdf().color_map,1) & ..
        length(gca().children)==0) then
        e = 0 // extension of the raw colormap to white (number of graded colors)
              // e < 0 => invert the order the native colormap part
              //          in order to get lighter colors for lower amplitudes
        if isdef("Colormap","l") then
            select Colormap
            case autumncolormap, e = -15
            case bonecolormap, e = -1
            case coolcolormap, e = 20
            case coppercolormap, e = -15
            case graycolormap, e = -1
            case hotcolormap, e = -1
            case hsvcolormap, e = 15
            case jetcolormap, e = -20
            case oceancolormap, e = -1
            case parulacolormap, e = -10,
            case pinkcolormap, e = -1
            case rainbowcolormap, e = 20
            case springcolormap, e = -10
            case summercolormap, e = -10
            case wintercolormap, e = -20
            else
                if type(Colormap)<>13
                    msg = _("%s: Argument #%d: Function expected.\n")
                    error(msprintf(msg, "mapsound", 5))
                end
                msg = _("%s: Argument #%d: A colormap function expected.\n")
                error(msprintf(msg, "mapsound", 5))
            end
            tmp = Colormap(100 - abs(e))
            if e < 0  then
                tmp = tmp($:-1:1,:)
                e = abs(e)
            end
            if e then
                tmp = [ones(e,3) - linspace(0, 1-1/e, e)'*([1 1 1] - tmp(1,:)); tmp]
            end
            // grid: color closest to grey50
            [?,k] = gsort(sum((ones(tmp)*0.5 - tmp).^2,"c"),"g","i")
            gridColor = k(find(k>e,1))
        else
            tmp = hotcolormap(118)($:-1:$*0.15,:);
            gridColor = 43
        end
        gcf().color_map = tmp;
    end

    // Graphical preparation
    ax = gca();
    ax.tight_limits = "on";
    ax.grid_position = "foreground"
    xlabel(_("Time [s]"), "fontsize",2)
    ylabel(_("Frequency [Hz]"), "fontsize",2)

    // Display the map
    //surf(dates, frequencies-(frequencies(2)-frequencies(1))/2, M)
    surf(dates, frequencies, M)
    ax.view = "2d";
    gce().color_mode = -2
    xgrid(gridColor)

    colorbar
    title(gce(), _("Amplitude"))
    xlabel(msprintf("%d × %d",size(M)), "fontsize",2)
    set("current_entity", ax.children)
    sca(ax)

    // Results
    lhs = argn(1)
    varargout = list()
    if lhs > 0 then
        varargout = list(dates)
    end
    if lhs > 1 then
        varargout($+1) = frequencies
    end
    if lhs > 2 then
        varargout($+1) = M
    end
endfunction
