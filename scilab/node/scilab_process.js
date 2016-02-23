var spawn = require('child_process').spawn;

var L = console.log;
var id = -1;
var io = [];

var debug = true;
var command_ready = false;
var graphic_ready = false;

process.on('message', function(msg){
    switch(msg.msgtype.toLowerCase()) {
        case 'initialization' :
            L('initialization ' + '(' + process.pid + ')');
            id = msg.id;
            
            //start command server to chat with Scilab
            var port = 10000+id;
            commandio = require('socket.io')(port);
            var commandaddr = 'http://127.0.0.1:' + port;
            
            commandio.on('connection', function (socket) {
                L("Scilab command connected");

                socket.on('command_end', function () {
                    L('command_end');
                    process.send({msgtype:'command_end'});
                });

                socket.on('disconnect', function () {
                    L('scilab command disconnected'+ '(' + process.pid + ')');
                    command_ready = false;
                });

                //send to server scilab is ready
                command_ready = true;
                if(graphic_ready) {
                    process.send({msgtype:"status", data:"ready"});
                }
           });

            //start graphic server to chat with graphic
            port += 1;
            graphicio = require('socket.io')(port);
            var graphicaddr = 'http://127.0.0.1:' + port;

            graphicio.on('connection', function (socket) {
                L("Scilab graphic connected");

                socket.on('graphic_create', function (msg) {
                    //L('graphic_create');
                    //L(msg);
                    process.send({msgtype:'graphic_create', data:msg});
                });

                socket.on('graphic_delete', function (msg) {
                    //L('graphic_delete');
                    //L(msg);
                    process.send({msgtype:'graphic_delete', data:msg});
                });

                socket.on('graphic_update', function (msg) {
                    //L('graphic_update');
                    //L(msg);
                    process.send({msgtype:'graphic_update', data:msg});
                });

                socket.on('disconnect', function () {
                    L('scilab graphic disconnected'+ '(' + process.pid + ')');
                    graphic_ready = false;
                });
                
                //send to server scilab is ready
                graphic_ready = true;
                if(command_ready) {
                    process.send({msgtype:"status", data:"ready"});
                }
            });

            //launch scilab with init command
            var app = process.env.SCI + "/bin/wscilex.exe";

            var scilabApp = spawn(app, ["-nw", "-commandaddr", commandaddr, "-graphicaddr", graphicaddr]);
            
            //to debug
            if(debug) {
/*
                scilabApp.stdout.on('data', function(data) {
                    L("scilab out :" + data.toString());
                });
*/                
                scilabApp.stderr.on('data', function(data) {
                    L("scilab err :" + data.toString());
                });
            }
            
           commandio.on('disconnect', function (socket) {
                L('scilab command disconnected 2');
            });  

            graphicio.on('disconnect', function (socket) {
                L('scilab graphic disconnected 2');
            });   
            
            break;
        case 'command' : 
            L('command (' + process.pid + ') : ' + msg.data);
            commandio.emit("command", {data:msg.data});
            break;
        case 'callback' : 
            graphicio.emit("callback", msg.data);
            break;
        case 'restart' :
            L('restart' + '(' + process.pid + ')');
            break;
        case 'quit' :
            L('quit' + '(' + process.pid + ')');
            commandio.emit("command", {data:'quit'});
            break;
        default :
            L('unknow message : ' + msg.msgtype);
        break;
    }
});
