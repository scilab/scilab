// connect to server
var socket;

resetLogger();
setInfo('');

function init() {
    socket = io();

    //socket events management
    socket.on('disconnect', function () {
        setInfo('Connection lost');
    });

    socket.on('status', function (msg) {
        switch(msg.data) {
            case 'ready':
            case 'reconnection':
                setInfo('Scilab is ready');
                resetLogger();
                //reset "scilab" page
                var parent = getScilab();
                parent.innerHTML = '';
                break;
            default:
                setInfo('Scilab is busy');
                break;
        }
        setLogger("<font color='black'>" +  msg.data + '</font>');
    });

    socket.on('graphic_create', function (msg) {
        //setLogger('<font color="green">' +  msg + '</font>');
        eval(msg);
    });

    socket.on('graphic_delete', function (msg) {
        //setLogger('<font color="red">' + msg + '</font>');
        eval(msg);
    });

    socket.on('graphic_update', function (msg) {
        //setLogger('<font color="orange">' +  msg + '</font>');
        eval(msg);
    });

    socket.on('graphic_reconnection', function (msg) {
        var scilab = getScilab();
        switch(msg) {
            case 'start':
                scilab.style.visibility = 'hidden';
                break;
            case 'end' :
                scilab.style.visibility = 'visible';
                break;
        }
    });
}

function force_reload() {
    getScilab().innerHTML = '';
    $('#taskbar').children().remove();
    socket.emit("force_reload");
}

function addWindow(win) {
    var $win = $('#scilab');

    //create taskbar buttons
    var $btn = $(document.createElement('button'));
    $btn.prop('type', 'button');
    $btn.dblclick(closeWindow);
    $btn.click(onSelectWindow);
    $btn.addClass('btn btn-default navbar-btn btn-window win-btn');
    $btn.prop('id', win.id + '_btn');
    $btn.text(win.id);
    $('#taskbar').append($btn);

    var $close = $(document.createElement('button'));
    $close.prop('type', 'button');
    $close.click(closeWindow);
    $close.addClass('btn btn-default navbar-btn btn-window close-btn glyphicon glyphicon-remove');
    $close.prop('id', win.id + '_close');
    $('#taskbar').append($close);

    $('#windows').append(win);

    //select me
    selectWindow(win);
}

function closeWindow() {
    socket.emit("closeWindow", {uid:parseInt(this.id.substring(3))});
}

function delWindow(uid) {
    //delete element and btn
    getJElementById(uid).remove();
    getJElementById(uid, '_btn').remove();
    getJElementById(uid, '_close').remove();
    selectWindow();
}

function onSelectWindow() {
    var idxstr = this.id.indexOf('_');
    var id = this.id.substring(0, idxstr);
    selectWindow(document.getElementById(id));
}

function selectWindow(win) {
    if(typeof win === 'undefined' ) {
        //find last window
        var wins = $('#scilab').children().last().get();
        if(wins.length == 0) {
            return;
        }
        
        win = wins[0];
    }
    
    $('#taskbar').children().not('#' + win.id + '_btn').css('background-color', '');
    $('#' + win.id + '_btn').css('background-color', 'skyblue');

    var $win = $('#scilab');
    $win.children().not('#' + win.id).css('display', 'none');
    $win.children('#' + win.id).css('display', 'block');
}

function onPushButton() {
    var id = getUID(this.id); //ignore "uid"
    socket.emit('callback', {uid:id});
}

function onCheckBox() {
    var id = getUID(this.id); //ignore "uid"
    socket.emit('callback', {uid:id, value:this.checked});
}

function onRadioButton() {
    var id = getUID(this.id); //ignore "uid"
    
    //send check state of all group components
    var children = $('input[name="' + this.name + '"]');
    for(var i = 0 ; i < children.length ; ++i) {
        socket.emit('callback', {uid:getUID(children[i].id), value:children[i].checked});
    }
}

function onListBox() {
    var id = getUID(this.id); //ignore "uid"
    var data = [];
    
    for(var i = 0 ; i < this.options.length; ++i) {
        if(this.options[i].selected) {
            data.push(i);
        }
    }
    socket.emit('callback', {uid:id, value:data});
}

function onComboBox() {
    var id = getUID(this.id); //ignore "uid"
    var data = [];
    
    for(var i = 0 ; i < this.options.length; ++i) {
        if(this.options[i].selected) {
            data.push(i);
            break;
        }
    }
    socket.emit('callback', {uid:id, value:data});
}

function onTab(e) {
    
    var $e = $(e.target)
    var index = $e.closest('li').index();
    var id = parseInt($e.closest('div').attr('id').substring(3));

    //console.log('onTab ' + (index+1));
    socket.emit('callback', {uid:id, value:index+1});
}

function onSlider() {
    var id = getUID(this.id); //ignore "uid"
    socket.emit('callback', {uid:id, value:parseFloat(this.value)});
}

function onEditBox() {
    var id = getUID(this.id); //ignore "uid"
    socket.emit('callback', {uid:id, value:this.value});
}

function onSpinner() {
    var id = getUID(this.id); //ignore "uid"
    socket.emit('callback', {uid:id, value:parseFloat(this.value)});
}

//tools
function unload() {
    socket.close();
}

function setInfo(data) {
    var info = document.getElementById('infobar');
    if(info) {
        info.innerHTML = data;
    }
}

function resetLogger() {
    var log = document.getElementById('logger');
    if(log) {
        log.innerHTML = '';
    }
}

function setLogger(data) {
    var log = document.getElementById('logger');
    if(log) {
        log.innerHTML += data + '</br>';
        log.scrollTop = log.scrollHeight;
    }
}
