/**
 * Created by CTXue on 14-10-17.
 */

var fs = require('fs');
var readlineSync = require('./node_modules/readline-sync');
var exec = require('child_process').exec;
var child = undefined;

var src_dir = './Source/';

var directory = readlineSync.question('Please enter dir name of the source code [under ./Source]: ');
var cod = readlineSync.question("Please enter the project name used for JSON output: ");
var path = src_dir + directory;

if (!fs.existsSync(path)) {
	console.log("Hey the directory does not exists, please double check.");
	return;
} 

fs.writeFileSync(path + '/config', fs.readFileSync('config'));
console.log('Finishing to copy the config file...');

if (child != undefined) child.kill();
child = exec("doxygen config", {cwd: path}, function (error, stdout, stderr) {
    //console.log(stdout);
    if (error !== null) {
        console.log('exec error: doxygen config -> Please double check');
				console.log(error);
    }
		console.log('The process is done.');
		
		var xmlPath = './Source/xml_';
		fs.renameSync(xmlPath, xmlPath + cod);
		
		exec("node parser.js " + cod, function(error, stdout, stderr){
			//console.log(stdout);
	    if (error !== null) {
	        console.log('exec error: node parser.js -> Please double check');
	    }
			console.log('Please check the JSON folder for the output.');
			var _xml = xmlPath + cod;
			exec("rm -r " + _xml);
		});
		
});

