const http = require('http');
const allurl = require('url');
const fs = require('fs');
const path = require('path');

http.createServer(function (request, response) {

    let parseObj = allurl.parse(request.url);
    let url = parseObj.pathname;

    var ContentType = {'Content-Type': 'text/html'};

    if(url === "/"){
        url = "index.html";
    }
    if(url.split(".")[1] === "css")
    {
        ContentType = {'Content-Type': 'text/css'};
    }
    if(url.split(".")[1] === "js")
    {
        ContentType = {'Content-Type': 'application/x-javascript'};
    }
    if(url.split(".")[1] === "wasm")
    {
        ContentType = {'Content-Type': 'application/wasm'};
    }
    
    fs.readFile(path.join(__dirname,url),function(err,data){
        if(err){
            console.log(err);
            response.writeHead(404,ContentType);
        }
        else{
            response.writeHead(200,ContentType);
            response.write(data.toString());
        }
        response.end();
    })
    
}).listen(8888);

// 终端打印如下信息
console.log('Server running at http://127.0.0.1:8888/');