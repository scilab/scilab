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

    //create taskbar button
    var $btn = $(document.createElement('button'));
    $btn.prop('type', 'button');
    $btn.dblclick(onDelWindow);
    $btn.click(onSelectWindow);
    $btn.addClass('btn btn-default navbar-btn btn-window');
    $btn.prop('id', win.id + '_btn');
    $btn.text(win.id);
    $('#taskbar').append($btn);

    //create window div
    $('#windows').append(win);
    
    selectWindow(win);
}

function onDelWindow() {
    var idxstr = this.id.indexOf('_');
    var id = this.id.substring(0, idxstr);
    delWindow(parseInt(this.id.substring(3)));
}

function delWindow(uid) {
    //delete element and btn
    getJElementById(uid).remove();
    getJElementById(uid, '_btn').remove();
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
    var __sibling__ = $('input[name="' + this.name + '"]');
    for(var i = 0 ; i < __sibling__.length ; ++i) {
        socket.emit('callback', {uid:getUID(__sibling__[i].id), value:__sibling__[i].checked});
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
    var __info__ = document.getElementById('infobar');
    if(__info__) {
        __info__.innerHTML = data;
    }
}

function resetLogger() {
    var __log__ = document.getElementById('logger');
    if(__log__) {
        __log__.innerHTML = '';
    }
}

function setLogger(data) {
    var __log__ = document.getElementById('logger');
    if(__log__) {
        __log__.innerHTML += data + '</br>';
        __log__.scrollTop = __log__.scrollHeight;
    }
}
