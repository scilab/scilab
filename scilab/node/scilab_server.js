var L = console.log;

var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

var fork = require('child_process').fork;

server.listen(1337);

app.get('/', function(req, res){
    L("req : " + req.toString());

    res.sendFile(__dirname + '/html/index.html');
    //messaging with web client
});


io.on('connection', function (socket) {
    L('connection');
    
    //start process
    var child = fork('./scilab_process.js');
    //messaging with scilab js
    child.on('message', function(msg){
        switch(msg.msgtype.toLowerCase()) {
            case 'command_end':
                socket.emit(msg.msgtype);
                break;
            case 'graphic_create':
            case 'graphic_delete':
            case 'graphic_update':
                //forward message
                socket.emit(msg.msgtype, msg);
                break;
            case 'status':
                if(msg.data == 'ready') {
                    //send ready message to client
                    socket.emit(msg.msgtype, msg);
                    //send execution of initial script to scilab
                    child.send({msgtype:'command', data:"exec('SCI/node/loader.sce', -1);"});
                }
            break;
            default:
                L('not manage : ' + msg.msgtype);
                break;
        }
    });

    child.send({msgtype:'initialization', id:child.pid});

    //receive command from client
    socket.on('command', function (data) {
        child.send({msgtype:'command', data:data});
    });
    
    socket.on('callback', function (data) {
        child.send({msgtype:'callback', data:data});
    });
    
    socket.on('disconnect', function () {
        L("disconnect");
        child.send({msgtype:'quit'});
    });

});

L('Server running at http://127.0.0.1:1337/');
