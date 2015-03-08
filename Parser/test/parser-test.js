
/**
 * Created by CTXue on 14-10-17.
 */

var fs = require('fs');
var assert = require("assert");
var expect = require('chai').expect;
var parseString = require('xml2js').parseString;
var readlineSync = require('readline-sync');
console.log("Codebase options: maryo, sod, bejew, sudoku");
var cod = readlineSync.question('Enter the input source codebase: ');

var inputCodeBase = '../Doxygen/xml_' + cod;

var staticInfo = [];
var writeOutput;
var fileList = fs.readdirSync(inputCodeBase);
var fileSize = undefined;
//the class list which contains all major classes in this codebase
var classList = [];
//the namespace used in front of class
var namespace;


/**
 * change the namespace with corresponding codebases
 */
var defineNameSpace = function(){
    switch(cod){
        case "bejew":
            namespace = "bejeweled::";
            break;
        case "maryo":
            namespace = "SMC::";
            break;
        case "sudoku":
            namespace = "";
            break;
        default:
            namespace = "";
            break;
    }
}

defineNameSpace();

/**
 * gather the class list from current codebase
 */
 var gatherClass = function() {

    fileList.forEach(function (file) {

        if (file.search('.xml') == -1) return;
        var path = inputCodeBase + "/" + file;

        var classFile = fs.readFileSync(path, 'utf8');
        parseString(classFile, function (err, result) {

            if (result.doxygen == undefined) return;
            var obj = result.doxygen.compounddef[0];
            switch (obj.$.kind) {
                case 'class':
                //console.dir("class: " + path);
                var output = obj.compoundname[0];
                classList.push(output);
                break;

                default :
                break;
            }
        });
    });
};

/**
 * end of gather the class list from current codebase
 */


/*
 * initialize the classList array
 */
gatherClass();
//console.log(classList);

/*
 * Refactor a file class object from the xml and output information as a JSON object
 */

var classInfo = function (_class) {

    //information will be used to return a json object from a the translated json file.
    var className = _class.compoundname[0];
    var method = [];
    var field = [];
    var inheritance;
    var dependency = [];
    var size;
    var output;

    fileSize ? size = fileSize : size = undefined;
    //console.log("ClassName: " + className + " Size: "+ size);
    //console.log(JSON.stringify(_class, null, 4));
    _class.basecompoundref ? inheritance = _class.basecompoundref[0]._ : inheritance="N/A";
    //entityArray, length in 2 or 1, each contains set of attributes or methods.
    var entityArray = _class.sectiondef;

    if (!entityArray) return;

    describe('Parsing the Class information: ', function(){
        it("should contain the expected className ", function(){
            assert(className != undefined);
            expect(className).to.be.a('String');
        });

        it("should contain the expected inheritance ", function(){
            assert(inheritance != undefined);
            expect(inheritance).to.be.a('String');
        });

        it("should output a correct entityArray contains attributes and/or methods", function(){
            assert(entityArray != undefined);
            expect(entityArray).to.be.a('Array');
            assert(entityArray.length >= 0);
        });
    });

    entityArray.forEach(function (entity) {

        //type of the entity: attrib vs func
        var entityType = entity.$.kind;
        // member definition [Array] inside of the entity
        var memDef = entity.memberdef;

    describe('Parsing the Class attributes and functions: ', function(){

        it("should contain the expected entityType (attrib vs func) ", function(){
            assert(entityType!= undefined);
            expect(entityType).to.be.a('String');
            //assert(entityType == 'attrib' || entityType == 'func');
        });

        it("should contain the expected member definition inside of a attrib or func", function(){
            assert(memDef  != undefined);
            expect(memDef ).to.be.a('Array');
            assert(entityArray.length > 0);
        });
    });

        // console.dir(entity);
        if (entityType.search('attrib') != -1) {
            //loop through each attribute inside of member definition array.
            memDef.forEach(function (attr) {

                //double check the attribute is a field variable
                var typeCheck = attr.$.kind;
                if (typeCheck != 'variable') return;

                if (field.indexOf(attr.name[0]) == -1){
                    field.push(attr.name[0]);
                }

                describe('Parsing attribute member definitions: ', function(){
                    it("should contain the variable definition ", function(){
                        assert(typeCheck != undefined);
                        expect(typeCheck ).to.be.a('String');
                        assert(typeCheck == 'variable');
                    });
                });

                var fieldType;
                //ref is an array contains the className information
                var typeCheckField = attr.type[0].ref ;

                //type check for class field, which outputs a class name if it has any.
                typeCheckField ? fieldType = typeCheckField[0]._ : fieldType = null;
                //console.log("Name: " + attr.name[0] + " ==> " + JSON.stringify(fieldType));
                var NS_fieldType = namespace + fieldType;
                if (classList.indexOf(NS_fieldType) > -1){
                    if (dependency.indexOf(NS_fieldType) == -1){
                    dependency.push(NS_fieldType);
                }
            }
                    //console.log("Name: " + attr.name[0] + " ==> " + JSON.stringify(fieldType));
            });


        } else if (entityType.search('func') != -1) {
            memDef.forEach(function (func) {
                //console.dir(func);
                var typeCheck = func.$.kind;
                if (typeCheck != 'function') return;

                if (method.indexOf(func.name[0]) == -1){
                    method.push(func.name[0]);
                }

                describe('Parsing method member definitions: ', function(){
                    it("should contain the function definition ", function(){
                        assert(typeCheck != undefined);
                        expect(typeCheck ).to.be.a('String');
                        assert(typeCheck == 'function');
                    });
                });

            });


        } else {
            console.log('Entity has not been defined yet');
        }

    });

    output = {
        'className': className, 'method': method, 'field': field, 'size': size,
        'inheritance': inheritance, 'dependency': dependency
    };

    //return the parsed info for single xml file as the output
    return output;


};

var fileInfo = function (_file) {
    //console.dir(_file.$);

};

var structInfo = function (_struct) {
    //console.dir(_struct.$);

};

console.log(" <--------  Start of Unit Test  -------->")

fileList.forEach(function (file) {

    if (file.search('.xml') == -1) return;




    //if (file != 'classc_spiny.xml') return;
    //
    //
    //

    var path = inputCodeBase + "/" + file;
    var stats = fs.statSync(path);
    fileSize = stats.size;

    var classFile = fs.readFileSync(path, 'utf8');
    parseString(classFile, function (err, result) {

        if (result.doxygen == undefined) return;

        var obj = result.doxygen.compounddef[0];

        describe('Parsing static XML files: ', function(){
            it("should contain a valid xml file ", function(){
                assert(fileSize > 0);
            });

            it("should contain a correct doxygen JSON object ", function(){
                assert(result.doxygen != undefined);
                expect(result.doxygen).to.be.a('Object');
            });

            it("should contain a correct compounddef JSON bject ", function(){
                assert(obj != undefined);
                expect(obj).to.be.a('Object');
            });


        });


        switch (obj.$.kind) {
            case 'class':
                //console.dir("class: " + path);
                var output = classInfo(obj);
                staticInfo.push(output);
                break;

            case 'file':
                //console.dir("file: " + path);
                fileInfo(obj);
                break;

            case 'struct':
                //console.dir("struct: " + path);
                structInfo(obj);
                break;

            default :
                console.dir("unknown: " + path);
                break;
        }


    });

});

writeOutput = {'staticInfo': staticInfo};

console.log(" <--------   End of Unit Test   -------->")

