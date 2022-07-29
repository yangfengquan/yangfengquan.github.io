"use strict"

function Router() {
    this.routes = {};
    this.currentUrl = '';
}
Router.prototype.route = function (path, callback) {
    this.routes[path] = callback || function () { };
};
Router.prototype.refresh = function () {
    document.getElementById(this.currentUrl)?.classList.remove("show");
    document.getElementById("page-result").classList.remove("show");
    document.getElementById("report").removeAttribute("href");
    //console.log(this.currentUrl)
    this.currentUrl = location.hash.slice(1) || '/';

    let curPage = document.getElementById(this.currentUrl);
    if (curPage != null) {
        curPage.classList.add("show");
        if (this.currentUrl != "/") {
            //document.getElementById("run").addEventListener("click", this.routes[this.currentUrl], false);
			document.getElementById("run").onclick = this.routes[this.currentUrl];
        }
    }
};
Router.prototype.init = function () {
    window.addEventListener('load', this.refresh.bind(this), false);
    window.addEventListener('hashchange', this.refresh.bind(this), false);
};
window.Router = new Router();
window.Router.init();

window.Router.route("page1", function () {
    var res = document.createElement("p");
    res.innerHTML = "page1 result";
    document.getElementById("page-result").appendChild(res);
    document.getElementById("page-result").classList.add("show");

    var txt = "1234\n中文12\t456\nAbvdf\t456";
    var blob = new Blob([txt], { type: "text/plain;charset=utf-8" ,endings:'native'});
    var link = document.getElementById("report");
    link.href = URL.createObjectURL(blob);
});

window.Router.route("page2", function () {
    alert("page2");
});

document.getElementById("open").onclick = function(){
    document.getElementById("file").click();  
}

document.getElementById("file").onchange = function(){
    var file_ele = document.getElementById("file")
    let reader = new FileReader();
    reader.readAsText(file_ele.files[0], "UTF-8");
    reader.onload = function(evt){
        let fileString = evt.target.result;
        console.log(fileString);
        const jsonObj = JSON.parse(fileString);
        console.log(jsonObj.c);
    }
}