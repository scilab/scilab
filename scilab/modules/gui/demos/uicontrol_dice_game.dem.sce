function idle_gui()
    global idle;
    f = fullfile(SCIHOME, "idle.sod")
    if isfile(f) then
        load(f);
        e = getdate("s");
        elaps = etime(getdate(e), getdate(idle.time));
        rolls = floor(elaps / 5);

        if rolls > 0 then
            dice = 0;
            for i = 1:idle.dice
                if idle.dices(i).enable
                    dice = dice + 1;
                end
            end

            r = int(rand(rolls, dice) * 6) + 1;

            gain = 0;
            for j = 1:rolls
                [g, msg] = idle_roll_gain(r(j, :))
                s = 0;
                for i = 1:idle.dice
                    if idle.dices(i).enable then
                        s1 = idle.dices(i).mult * r(j, i);
                        s = s + s1;
                    end
                end

                gain = gain + g * s;
            end


            msg = sprintf("During your absence you earned %s", formatEng(gain));
            messagebox(msg, "Game of Dice", "info", "modal");

            idle.gain = idle.gain + gain;
        end
    else
        //init rand
        rand("seed", getdate("s"))
        idle.first_run = %t;
        idle.path = get_absolute_file_path("uicontrol_dice_game.dem.sce");
        idle.cumul_gain = 0;
        idle.gain = 0;
        idle.autoroll = %f;
        idle.quit = %f;
        idle.reset = %f;
        idle.action.type = [];
        idle.history = [];

        idle.colors = [0.8666667   0.4666667   0.4313725
                       0.8784314   0.5058824   0.427451
                       0.8862745   0.5333333   0.4235294
                       0.8980392   0.572549    0.4196078
                       0.9058824   0.6039216   0.4117647
                       0.9137255   0.6352941   0.4078431
                       0.9254902   0.6745098   0.4039216
                       0.9019608   0.6784314   0.3803922
                       0.9137255   0.7215686   0.3803922
                       0.9529412   0.772549    0.3882353
                       0.9607843   0.8078431   0.3843137
                       0.8862745   0.7882353   0.3960784
                       0.8313725   0.7843137   0.4156863
                       0.7686275   0.772549    0.427451
                       0.6901961   0.745098    0.4313725
                       0.6431373   0.7529412   0.4509804
                       0.5803922   0.7411765   0.4666667
                       0.5176471   0.7333333   0.4823529
                       0.4509804   0.7215686   0.4941176
                       0.3882353   0.7137255   0.5098039
                       0.3411765   0.7333333   0.5411765]

        idle.dice = 1;
        idle.dices = [];
        idle.dices($ + 1) = struct(...
            "base", 10 / 1.08, ...
            "cost", 1.08, ...
            "mult", 1, ...
            "multCoef", 1, ...
            "enable", %t, ...
            "level", 1);

        idle.dices($ + 1) = struct(...
            "base", 100, ...
            "cost", 1.08, ...
            "mult", 2, ...
            "multCoef", 2, ...
            "enable", %f, ...
            "level", 0);

        idle.dices($ + 1) = struct(...
            "base", 1000 * 1.08, ...
            "cost", 1.08, ...
            "mult", 4, ...
            "multCoef", 4, ...
            "enable", %f, ...
            "level", 0);

        idle.dices($ + 1) = struct(...
            "base", 10000 * 1.08, ...
            "cost", 1.08, ...
            "mult", 8, ...
            "multCoef", 8, ...
            "enable", %f, ...
            "level", 0);

        idle.dices($ + 1) = struct(...
            "base", 100000 * 1.08, ...
            "cost", 1.08, ...
            "mult", 16, ...
            "multCoef", 16, ...
            "enable", %f, ...
            "level", 0);
    end

    //always at startup
    idle.multiplier = 1;

    f = figure(...
        "dockable", "off", ...
        "position", [10 250 800 600], ...
        "infobar_visible", "off", ...
        "toolbar", "none", ...
        "menubar_visible", "on", ...
        "menubar", "none", ...
        "default_axes", "off", ...
        "resize", "off", ...
        "layout", "border", ...
        "figure_name", "Game of Dice", ...
        "background", -2, ...
        "visible", "off", ...
        "closerequestfcn", "global idle;idle.quit = %t;", ...
        "tag", "idle_fig");

    fr = uicontrol(f, ...
        "style", "frame", ...
        "backgroundcolor", [1 1 1], ...
        "tag", "idle_top", ...
        "constraints", createConstraints("border", "top", [0 50]));

    uicontrol(fr, ...
        "style", "text", ...
        "fontsize", 40, ...
        "fontweight", "bold", ...
        "string", formatEng(idle.gain), ...
        "horizontalalignment", "center", ...
        "backgroundcolor", [1 1 1], ...
        "tag", "idle_total", ...
        "position", [100 5 600 40])

    u = idle_create_button(fr, "idle_help", [10 10 30 30]);
    set(u, "icon", "help-browser");
    set(u, "fontsize", 16);
    set(u, "fontweight", "bold");
    set(u, "callback", "global idle;idle.action.type = ""help"";");
    set(u, "callback_type", 10);

    fr = uicontrol(f, ...
        "style", "frame", ...
        "backgroundcolor", [1 1 1], ...
        "tag", "idle_bottom", ...
        "constraints", createConstraints("border", "bottom", [0 50]));

    u = idle_create_button(fr, "idle_roll", [350 10 100 30]);
    set(u, "string", "Roll !");
    set(u, "fontsize", 16);
    set(u, "fontweight", "bold");
    set(u, "callback", "global idle;idle.action.type = ""roll"";");
    set(u, "callback_type", 10);

    u = idle_create_button(fr, "idle_btn_history", [690 10 100 30]);
    set(u, "string", "History");
    set(u, "fontsize", 16);
    set(u, "callback", "global idle;idle.action.type = ""history"";");
    set(u, "callback_type", 10);

    u = idle_create_button(fr, "idle_btn_multi", [150 10 100 30]);
    set(u, "string", sprintf("x %d", idle.multiplier));
    set(u, "fontsize", 16);
    set(u, "callback", "global idle;idle.action.type = ""multiplier"";");
    set(u, "callback_type", 10);

    uicontrol(fr, ...
            "style", "text", ...
            "horizontalalignment", "center", ...
            "fontweight", "bold", ...
            "string", "Inspired by", ...
            "backgroundcolor", [1 1 1], ...
            "position", [10 20 80 20]);

    uicontrol(fr, ...
        "style", "text", ...
        "horizontalalignment", "center", ...
        "string", "<html><a href=""https://luts91.github.io/idle-dices"">Idle Dice</a></html>", ...
        "backgroundcolor", [1 1 1], ...
        "position", [10 0 80 20]);

    v = "off";
    if  idle.autoroll then
        v = "on";
    end

    fr1 = uicontrol(fr, ...
        "style", "frame", ...
        "visible", v, ...
        "border", createBorder("line", "#000000", 1), ...
        "backgroundcolor", [1 1 1], ...
        "tag", "idle_autoroll", ...
        "position", [350 13 104 24]);

    uicontrol(fr1, ...
        "style", "frame", ...
        "tag", "idle_progress", ...
        "position", [2 2 0 20]);

    fr = uicontrol(f, ...
        "style", "frame", ...
        "backgroundcolor", [1 1 1], ...
        "tag", "idle_center");

    uicontrol(fr, ...
        "style", "text", ...
        "string", "", ...
        "relief", "default", ...
        "horizontalalignment", "center", ...
        "fontsize", 32, ...
        "backgroundcolor", [1 1 1], ...
        "fontweight", "bold", ...
        "tag", "roll_msg", ...
        "position", [200 400 400 75]);

    uicontrol(fr, ...
        "style", "text", ...
        "string", "", ...
        "horizontalalignment", "center", ...
        "fontsize", 32, ...
        "backgroundcolor", [1 1 1], ...
        "fontweight", "bold", ...
        "tag", "roll_gain", ...
        "position", [200 350 400 50]);

    for i = 1:idle.dice
        idle_dice_gui(i);
    end

    if idle.dices(1).level > 1 then
        idle_show_autoroll();
    end

    set("idle_fig", "visible", "on");
endfunction

function u = idle_create_button(parent, tag, pos)

    fr = uicontrol(parent, ...
        "style", "frame", ...
        "position", pos, ...
        "border", createBorder("etched", "raised", "#d0d0d0", "#202020"), ...
        "backgroundcolor", [1 1 1], ...
        "tag", sprintf("%s_frame", tag));

    u = uicontrol(fr, ...
        "style", "pushbutton", ...
        "tag", tag, ...
        "backgroundcolor", [1 1 1], ...
        "position", [1 1 pos(3:4) - 2]);
endfunction

function idle_show_history()

    if get("idle_history") == [] then
        f = get("idle_fig");

        fr = uicontrol(f, ...
            "style", "frame", ...
            "backgroundcolor", [1 1 1], ...
            "constraints", createConstraints("border", "right", [200 0]), ...
            "tag", "idle_history");

        uicontrol(fr, ...
            "style", "listbox", ...
            "position", [0 0 200 500], ...
            "fontname", "consolas", ...
            "string", string((1:100)'), ...
            "visible", "off", ...
            "tag", "idle_history_list");

            pos = get("idle_fig", "position");
            pos(3) = 1000;
            set("idle_fig", "position", pos);
    else
        delete(get("idle_history"));
        pos = get("idle_fig", "position");
        pos(3) = 800;
        set("idle_fig", "position", pos);
        return;
    end
endfunction

function idle_refresh_history()
    global idle;

    if get("idle_history_list") == [] then
        return;
    end

    set("idle_history_list", "visible", "off");
    history_str = [];
    history = idle.history;
    h = history(:, 1:5);

    for i = 1:size(h, "r")
        hi = h(i, :);
        hi = hi(hi <> 0);
        s = strcat(string(hi), " - ");
        s = s + " => " + string(history(i, 6));
        history_str($ + 1) = s;
    end

    set("idle_history_list", "string", history_str);
    set("idle_history_list", "listboxtop", size(history_str, "r"));
    set("idle_history_list", "visible", "on");
endfunction

function idle_show_autoroll()
    global idle;
    idle.autoroll = %t;

    set("idle_roll_frame", "position", [460 10 100 30]);
    set("idle_autoroll", "visible", "on");
    tic(); //start timer
endfunction

function idle_dice_gui(i)
    global idle;
    x = idle.dice
    st = idle.dices(i);

    offset = (800 - (x * 110))/ 2;
    f = get("idle_center");

    if get(sprintf("dice_%d", i)) <> [] then
        delete(get(sprintf("dice_%d", i)));
    end

    //frame
    fr = uicontrol(f, ...
        "style", "frame", ...
        "relief", "default", ...
        "tag", sprintf("dice_%d", i), ...
        "backgroundcolor", [1 1 1], ...
        "position", [offset + ((i - 1)*110), 150 110 200]);

    u = idle_create_button(fr, sprintf("btn_%d", i), [0 10 100 30]);
    set(u, "enable", "off");
    set(u, "string", formatEng(st.base * st.cost ^ (st.level)));
    set(u, "foregroundcolor", [0.75 0 0]);
    set(u, "callback", sprintf("global idle;idle.action.type = ""upgrade"";idle.action.dice = %d", i));
    set(u, "callback_type", 10);

    //text 1
    uicontrol(fr, ...
        "style", "text", ...
        "horizontalalignment", "center", ...
        "backgroundcolor", [1 1 1], ...
        "tag", sprintf("label_1_%d", i), ...
        "position", [10 40 80 15]);

    //text 2
    uicontrol(fr, ...
        "style", "text", ...
        "horizontalalignment", "center", ...
        "backgroundcolor", [1 1 1], ...
        "tag", sprintf("label_2_%d", i), ...
        "position", [5 55 90 15]);

    //visible only if enable
    v = "off";
    if st.enable then
        v = "on";
    end

    //image
    uicontrol(fr, ...
        "style", "image", ...
        "string", fullfile(idle.path, "images", "dice-1.png"), ...
        "position", [10 80 80 80], ...
        "backgroundcolor", [1 1 1], ...
        "tag", sprintf("image_%d", i), ...
        "horizontalalignment", "center", ...
        "visible", v, ...
        "verticalalignment", "middle");

    //text 3
    uicontrol(fr, ...
        "style", "text", ...
        "string", "0", ...
        "horizontalalignment", "center", ...
        "backgroundcolor", [1 1 1], ...
        "tag", sprintf("label_3_%d", i), ...
        "fontsize", 16, ...
        "visible", v, ...
        "position", [10 170 80 20]);

    idle_update_dice(i);
endfunction

function idle_move_dice()
    global idle;
    dice = idle.dice;

    oldOff = (800 - (dice * 110)) / 2;
    newOff = (800 - ((dice + 1) * 110)) / 2;

    posX = [];
    for i = 1:dice
        posX($ + 1, 1)  = get(sprintf("dice_%i", i), "position")(1);
        posX($    , 2)  = newOff + ((i - 1) * 110);
    end

    offsets = linspace(posX(:, 1), posX(:, 2), 10);

    for off = 1:size(offsets, "c")
        for i = 1:dice
            d = sprintf("dice_%i", i);
            pos = get(d, "position");
            pos(1) = offsets(i, off);
            set(d, "position", pos);
        end

        sleep(100);
    end

endfunction

function idle_buy(x)
    global idle;

    //reduce money
    cost = idle_dice_upgrade_cost(x);
    if cost > idle.gain then
        return;
    end
    idle.gain = idle.gain - cost;
    set("idle_total", "string", formatEng(idle.gain));

    //upgrade dice
    idle.dices(x).level = min(idle.dices(x).level + idle.multiplier, 100);
    idle.dices(x).mult = idle.dices(x).level * idle.dices(x).multCoef;

    //enable autoroll after 1st upgrade of 1st dice
    if idle.autoroll == %f && x == 1 && idle.dices(x).level > 1 then
        idle_show_autoroll();
    end

    idle_update_dice(x);

    if x < 5 && x == idle.dice && idle.dices(x + 1).enable == %f && idle.dices(x).level >= 10 then
        idle_move_dice();
        idle.dice = idle.dice + 1;
        idle_dice_gui(x + 1);
    end

    victory = and(list2vec(idle.dices.level) == 100);
    if victory then
        msg = ["Victory !";
               "";
               "Do you want to reset progression?"];
        answer = messagebox(msg, "Game of Dice", "question", ["Yes", "No"], "modal");
        if answer == 1 then
            idle.reset = %t;
            idle.quit = %t;
        end
    end
endfunction

function c = idle_dice_upgrade_cost(x)
    global idle;

    m = idle.multiplier;
    d = idle.dices(x);

    //propagation of the round error is voluntary
    c = sum(round(d.base * d.cost ** (d.level : min(d.level + m-1, 100))));
endfunction

function idle_update_dice(x)
    global idle;

    if idle.dices(x).enable == %f && idle.dices(x).level > 0 then
        idle.dices(x).enable = %t;
        //show image and 3rd label
        set(sprintf("image_%d", x), "visible", "on");
        set(sprintf("label_3_%d", x), "visible", "on");
    end

    idle_update_dices_price();

    set(sprintf("label_1_%d", x), "string", sprintf("Level: %d", idle.dices(x).level));
    set(sprintf("label_2_%d", x), "string", sprintf("Mult: %d", idle.dices(x).mult));

    idle_check_dice_price();
endfunction

function idle_update_dices_price()
    global idle;

    set("idle_btn_multi", "string", sprintf("x %d", idle.multiplier));

    for i = 1:idle.dice
        st = idle.dices(i);
        tag = sprintf("btn_%d", i);
        if get(tag) <> [] then
            if st.level >= 100 then
                set(sprintf("btn_%d", i), "string", "MAX");
            else
                //update price to new level
                price = idle_dice_upgrade_cost(i);
                set(sprintf("btn_%d", i), "string", formatEng(price));
            end
        end
    end
endfunction

function str = formatEng(x)
    if x == 0 then
        str = "0";
        return;
    end

    e = 6 * floor(log10(x) / 6);
    if e then
        y = x / (10 ^ e);
        str = sprintf("%.3fe%d", y, e)
    else
        str = sprintf("%0.f", x);
    end
endfunction

function [g, msg] = idle_roll_gain(r)
    g = 1;
    rsort = gsort(r, "c", "i");
    [a, b, ?, c] = unique(rsort);
    c = gsort(c);
    s = length(rsort);

    if s >= 5 && c(1) == 5 then
        g = 500;
        msg = "FIVE !"
    elseif s >= 4 && c(1) == 4 then
        g = 100;
        msg = "Four"
    elseif s == 5 && c(1) == 3 && c(2) == 2 then
        g = 50;
        msg = "Fullhouse"
    elseif s == 5 && (and(rsort == 1:5) || and(rsort == 2:6)) then
        g = 40;
        msg = "Straight"
    elseif s >= 3 && c(1) == 3 then
        g = 20;
        msg = "Three"
    elseif s >= 4 && c(1) == 2 && c(2) == 2 then
        g = 10;
        msg = "Two pair"
    elseif s >= 2 && c(1) == 2 then
        g = 5;
        msg = "One pair"
    else
        g = 1;
        msg = ""
    end

    msg = sprintf("%s x%d", msg, g);
endfunction

function idle_check_dice_price()
    global idle;
    for i = 1:idle.dice
        tag = sprintf("btn_%d", i);
        if get(tag) == [] then
            return;
        end
        if idle.dices(i).level >= 100 then
            set(tag, "foregroundcolor", [0 0.5 0]);
            set(tag, "backgroundcolor", [0.95 0.95 0.95]);
                set(tag, "fontweight", "bold");
            set(tag, "enable", "off");
        else
            if idle.gain >= idle_dice_upgrade_cost(i) then
                set(tag, "foregroundcolor", [0 0.5 0]);
                set(tag, "backgroundcolor", [1 1 1]);
                set(tag, "fontweight", "bold");
                set(tag, "enable", "on");
            else
                set(tag, "foregroundcolor", [0.75 0 0]);
                set(tag, "backgroundcolor", [0.95 0.95 0.95]);
                set(tag, "fontweight", "normal");
                set(tag, "enable", "off");
            end
        end
    end
endfunction

function idle_roll()
    global idle;

    set("idle_roll", "enable", "off");
    set("roll_msg", "string", "");
    set("roll_gain", "string", "");

    dice = 0;
    for i = 1:idle.dice
        if idle.dices(i).enable
            set(sprintf("label_3_%d", i), "string", "");
            dice = dice + 1;
        end
    end

    global idle;
    roll_image = 6;
    r = int(rand(roll_image, dice) * 6) + 1;

    for i = 1:roll_image
        for j = 1:idle.dice
            if idle.dices(j).enable then
                set(sprintf("image_%d", j), "string", fullfile(idle.path, "images", sprintf("dice-%d.png", r(i, j))));
            end
        end

        sleep(50);
    end

    //keep only last roll
    r = r($, :);
    [gain, msg] = idle_roll_gain(r);
    set("roll_msg", "string", msg);

    s = 0;
    for i = 1:idle.dice
        if idle.dices(i).enable then
            s1 = idle.dices(i).mult * r(i);
            set(sprintf("label_3_%d", i), "string", formatEng(s1));
            s = s + s1;
        end
    end

    total = s * gain;
    msg_gain = sprintf("%s x %d = %s", formatEng(s), gain, formatEng(total));
    set("roll_gain", "string", msg_gain);
    set("idle_roll", "enable", "on");

    idle.gain = idle.gain + total;
    idle.cumul_gain = idle.cumul_gain + total;

    idle_add_history(r, total);
    set("idle_total", "string", formatEng(idle.gain));

    idle_check_dice_price();
endfunction

function idle_add_history(h, val)
    global idle;

    if length(h) < 5 then
        h(5) = 0;
    end

    h($+1) = val;

    idle.history($ + 1, :) = h;
    //keep the 100 last entries
    idle.history(1:$-100, :) = [];
endfunction

function idle_show_help()
    msg = ["Welcome in ""Game of Dice""";
           ""
           "This demo is used to show the capabilities of";
           "`uicontrols` and their callbacks";
           ""
           "Goal: ";
           "-  Reach level 100 for each of the 5 dices";
           ""
           "Info: ";
           "After the first purchase, the game can roll alone.";
           "but you can anticipate the draw by clicking on ""Roll !""";
           ];

    messagebox(msg, "Game of Dice", "info", "modal");
endfunction

//almost like a game event loop
//manage events from callback or timer
function idle_event_loop()
    global idle;

    printf("\nThe Scilab console cannot be used during the game.\n");

    while idle.quit == %f
        action = idle.action;
        idle.action = struct("type", []);

        select(action.type)
        case "roll"
            set("idle_progress", "position", [2 2 0 20]);
            idle_roll();

            idle_refresh_history();
            tic();
        case "upgrade"
            dice = action.dice;
            idle_buy(dice);
        case "history"
            idle_show_history();
            idle_refresh_history();
        case "multiplier"
            select(idle.multiplier)
            case 1
                idle.multiplier = 10;
            case 10
                idle.multiplier = 100;
            case 100
                idle.multiplier = 1;
            end

            idle_update_dices_price();
            idle_check_dice_price();
        case "help"
            idle_show_help();
        end

        if idle.autoroll then
            t = toc();
            p = round((t / 5) * 100);
            if p >= 100 then
                idle.action.type = "roll";
                set("idle_progress", "position", [2 2 0 20]);
                set("idle_progress", "backgroundcolor", idle.colors(1, :));
                tic();
            else
                set("idle_progress", "position", [2 2 p 20]);
                set("idle_progress", "backgroundcolor", idle.colors(round(p / 5)+1, :));
            end
        end

        //relax CPU
        sleep(10);
    end

    idle.time = getdate("s");
    //to allow restart
    idle.quit = %f;
    //close window
    close(get("idle_fig"));
endfunction

function idle_save()
    global idle;
    save(fullfile(SCIHOME, "idle.sod"), "idle");
endfunction

function idle_hardreset()
    deletefile(fullfile(SCIHOME, "idle.sod"));
endfunction

clc;
global idle;
idle_gui();
if idle.first_run then
    idle_show_help();
    idle.first_run = %f;
end

idle_event_loop();

global idle;
if idle.reset == %t then
    idle_hardreset();
else
    idle_save();
end

clearglobal idle;
clear idle;
clear idle_create_button;
clear idle_show_history;
clear idle_refresh_history;
clear idle_show_autoroll;
clear idle_dice_gui;
clear idle_move_dice;
clear idle_buy;
clear idle_update_dice;
clear idle_dice_upgrade_cost;
clear idle_update_dices_price;
clear formatEng;
clear idle_roll_gain;
clear idle_check_dice_price;
clear idle_roll;
clear idle_add_history;
clear idle_event_loop;
clear idle_save;
clear idle_gui;
clear idle_show_help;
clear idle_hardreset;
