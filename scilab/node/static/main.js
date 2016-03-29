// connect to server
var socket = io();

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
            var parent = document.getElementById('scilab');
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
    var scilab = document.getElementById('scilab');
    switch(msg) {
        case 'start':
            console.log('hidden');
            scilab.style.visibility = 'hidden';
            break;
        case 'end' :
            console.log('visible');
            scilab.style.visibility = 'visible';
            break;
    }
});

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
    socket.emit('callback', {uid:id, value:this.checked});
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
}

function onTab() {
    var li = this.parentNode;
    var ul = li.parentNode;

    var k = 0;
    var elem = li;
    while(elem=elem.previousSibling){k++;}

    var id = parseInt(ul.id.substring(3)); //ignore "uid"
    socket.emit('callback', {uid:id, value:k});
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
    document.getElementById('infobar').innerHTML = data;
}

function resetLogger() {
    var objDiv = document.getElementById('logger');
    objDiv.innerHTML = '';
}

function setLogger(data) {
    var objDiv = document.getElementById('logger');
    objDiv.innerHTML += data + '</br>';
    objDiv.scrollTop = objDiv.scrollHeight;
}
