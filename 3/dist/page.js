"use strict"

function Router() {
    this.currentUrl = '';
}

Router.prototype.refresh = function () {   
    document.getElementById(this.currentUrl)?.classList.remove("show");
    document.getElementById("report").removeAttribute("href");
    this.currentUrl = location.hash.slice(1) || '/';
    document.getElementById(this.currentUrl)?.classList.add("show");
};

Router.prototype.init = function () {
    window.addEventListener('load', this.refresh.bind(this), false);
    window.addEventListener('hashchange', this.refresh.bind(this), false);
};
window.Router = new Router();
window.Router.init();

document.getElementById("tog-menu").onclick = function () {
    let menu = document.getElementById("menu");
    if (menu.style.display == "block") {
        menu.style.display = "none";
    } else {
        menu.style.display = "block";
    }
}

function newFile() {
    document.getElementById("menu").style.display = "block";
    alert('选择菜单项。');
}

document.getElementById("open").onclick = function(){
    document.getElementById("file").click();  
}

document.getElementById("file").onchange = function(){
    let reader = new FileReader();
    reader.readAsText(this.files[0], "UTF-8");
    reader.onload = function(evt){
        let fileString = evt.target.result;
        const jsonObj = JSON.parse(fileString);

        document.getElementById(window.Router.currentUrl).classList.remove("show");
        location.hash = "#" + jsonObj.page;
        window.Router.currentUrl = jsonObj.page;
        let currentPage = document.getElementById(jsonObj.page);
        let currentRow = currentPage.getElementsByClassName("form")[0].firstElementChild;
        for (let i = 0; i < jsonObj.data.length; i++) {
            const row = jsonObj.data[i];
            let inputs = currentRow.getElementsByTagName("input");
            for (let j = 0; j < row.length; j++) {
                const val = row[j];
                inputs[j].value = val;
            }
            if (!(currentRow = currentRow.nextElementSibling) && i !=  jsonObj.data.length - 1) {
                currentRow = addRow();
            }
        }
    }
}

function save() {
    let dataObj = new Object();
    dataObj.page = window.Router.currentUrl;
    dataObj.data = new Array();

    let currentPage = document.getElementById(window.Router.currentUrl);
    let currentRow = currentPage.getElementsByClassName("form")[0].firstElementChild;
    
    do {
        let row = new Array();
        let inputs = currentRow.getElementsByTagName("input");
        for (let i = 0; i < inputs.length; i++) {
            const val = inputs[i].value;
            row.push(val);
        }
        dataObj.data.push(row);
    } while (currentRow = currentRow.nextElementSibling);

    let link = document.getElementById("save-link");
    link.href = downLoadLink(JSON.stringify(dataObj));
    link.click();
}

function downLoadLink(text) {
    var blob = new Blob([text], { type: "text/plain;charset=utf-8" ,endings:'native'});
    return URL.createObjectURL(blob);
}

function addRow() {
    let currentPage = document.getElementById(window.Router.currentUrl);
    let formEle = currentPage.getElementsByClassName("form")[0];
    let lastRowEle = formEle.lastElementChild;
    let nextRowEle = lastRowEle.cloneNode(true);
    formEle.appendChild(nextRowEle);
    return nextRowEle;
}

function delRow(e) {
    let rowEle = e.target.parentNode.parentNode;//row
    if (rowEle.parentNode.childElementCount > 1) {
        rowEle.parentNode.removeChild(rowEle);
    }
}

function run() {
    window.Runner.methods[window.Router.currentUrl]();
}

function Runer() {
    this.methods = {};
}

Runer.prototype.method = function (page, callback) {
    this.methods[page] = callback || function () { };
};

window.Runner = new Runer();

window.Runner.method ("/", function () {
    alert("未选择功能。");
});

window.Runner.method ("pipe-material-sum", function () {
    let pipe = new Pipe();

    let rowEle = event.target.parentNode.parentNode;
    let inputs = rowEle.getElementsByTagName("input");
    pipe.do_ = Number(inputs[0].value) / 1000;
    pipe.thk = Number(inputs[1].value) / 1000;
    pipe.insulThk = Number(inputs[2].value) / 1000;
    let length = Number(inputs[3].value);

    rowEle.children[4].innerHTML = pipe.weight().toFixed(2);
    rowEle.children[5].innerHTML = (pipe.weight()* length).toFixed(2);
    rowEle.children[6].innerHTML = (pipe.area() * length).toFixed(2);
    rowEle.children[7].innerHTML = (pipe.insulVolume() * length).toFixed(2);
    rowEle.children[8].innerHTML = (pipe.cladArea() * length).toFixed(2);

    let sum = [0, 0, 0, 0];
    let currentRow = rowEle.parentNode.firstElementChild;
    do {
        sum[0] += Number(currentRow.children[5].innerHTML);
        sum[1] += Number(currentRow.children[6].innerHTML);
        sum[2] += Number(currentRow.children[7].innerHTML);
        sum[3] += Number(currentRow.children[8].innerHTML);
    } while (currentRow = currentRow.nextElementSibling);
    
    let currentSum = rowEle.parentNode.nextElementSibling.firstElementChild.children[5];
    for (let i = 0; i < sum.length; i++) {
        currentSum.innerHTML = sum[i].toFixed(2);
        currentSum = currentSum.nextElementSibling;
    }

    let table = rowEle.parentNode.parentNode.cloneNode(true);
    table.border = "1";
    for (let i = 0; i < table.rows.length; i++) {
        for (let j = 0; j < 4 && i > 0 && i < table.rows.length - 1; j++) { 
            const cell = table.rows[i].cells[j];
            cell.innerHTML = cell.children[0].value;  
        }
        table.rows[i].deleteCell(9);
    }
    let content = `<!DOCTYPE html><html><head><meta charset="UTF-8"></head><body>` + table.outerHTML
        + "</body></html>";
    //let blob = new Blob([content], {type: "application/ms-word;charset=gb2312"});
    //document.getElementById("report").href = URL.createObjectURL(blob);
    document.getElementById("report").href = downLoadLink(content);
});

window.Runner.method ("pipe-weigth", function () {
    let pipe = new Pipe();

    let rowEle = event.target.parentNode.parentNode;
    let inputs = rowEle.getElementsByTagName("input");
    pipe.do_ = Number(inputs[0].value) / 1000;
    pipe.thk = Number(inputs[1].value) / 1000;
    pipe.insulThk = Number(inputs[2].value) / 1000;
    pipe.insulDensity = Number(inputs[3].value);
    pipe.cladThk = Number(inputs[4].value) / 1000;
    pipe.cladDensity = Number(inputs[5].value);
    let length = Number(inputs[6].value);

    rowEle.children[7].innerHTML = pipe.weight().toFixed(2);
    rowEle.children[8].innerHTML = (pipe.weight()* length).toFixed(2);
    rowEle.children[9].innerHTML = (pipe.insulWeight() * length).toFixed(2);
    rowEle.children[10].innerHTML = (pipe.cladWeight() * length).toFixed(2);
    //rowEle.children[11].innerHTML = (pipe.weight() + pipe.insulWeight() + pipe.cladWeight()).toFixed(2);
    rowEle.children[11].innerHTML = ((pipe.weight() + pipe.insulWeight() + pipe.cladWeight()) * length).toFixed(2);

    let sum = [0, 0, 0, 0];
    let currentRow = rowEle.parentNode.firstElementChild;
    do {
        sum[0] += Number(currentRow.children[8].innerHTML);
        sum[1] += Number(currentRow.children[9].innerHTML);
        sum[2] += Number(currentRow.children[10].innerHTML);
        sum[3] += Number(currentRow.children[11].innerHTML);
    } while (currentRow = currentRow.nextElementSibling);
    
    let currentSum = rowEle.parentNode.nextElementSibling.firstElementChild.children[8];
    for (let i = 0; i < sum.length; i++) {
        currentSum.innerHTML = sum[i].toFixed(2);
        currentSum = currentSum.nextElementSibling;
    }

    let table = rowEle.parentNode.parentNode.cloneNode(true);
    table.border = "1";
    for (let i = 0; i < table.rows.length; i++) {
        for (let j = 0; j < 7 && i > 0 && i < table.rows.length - 1; j++) { 
            const cell = table.rows[i].cells[j];
            cell.innerHTML = cell.children[0].value;  
        }
        table.rows[i].deleteCell(12);
    }
    let content = `<!DOCTYPE html><html><head><meta charset="UTF-8"></head><body>` + table.outerHTML
        + "</body></html>";
    document.getElementById("report").href = downLoadLink(content);
});