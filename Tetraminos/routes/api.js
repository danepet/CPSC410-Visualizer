var express = require('express');
var router = express.Router();
var sys = require('sys');
var exec = require('child_process').exec;
var execFile = require("child_process").execFile
var child = undefined;

/* GET users listing. */
router.get('/', function (req, res) {
    res.send('respond with a resource');
});

router.get('/pong', function (req, res) {
    //if (process.getuid) {
    //    console.log('Current uid: ' + process.getuid());
    //}
    if (child != undefined) child.kill();
    var output = undefined;
    child = exec("./mario.out", {cwd: "static_lib"}, function (error, stdout, stderr) {
        //sys.print('stdout: ' + stdout);
        //sys.print('stderr: ' + stderr);
        //output = stdout.split("=");
        console.log(stdout);
        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });


    res.json({title: 'Pong Analyzer Starting...', content: null });

});

router.get('/racing', function (req, res) {
    if (child != undefined) child.kill();
    var output = undefined;
    child = exec("Bejeweled.exe", {cwd: "static_lib/Games/Bejeweled"}, function (error, stdout, stderr) {
        console.log(stdout);
        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });


    res.json({title: 'Racing Analyzer Starting...', content: null });

});

router.get('/pokemon', function (req, res) {
    if (child != undefined) child.kill();
    var output = undefined;
    child = exec('start cmd.exe /k "SOD.exe"', {cwd: "static_lib/Games/SOD"}, function (error, stdout, stderr) {
        console.log(stdout);
        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });

    res.json({title: 'Pokemon Analyzer Starting...', content: null });;

});



module.exports = router;
