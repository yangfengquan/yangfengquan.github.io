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
    //let form = document.getElementById("page1_form");
    let rows = new Array();
    let rowsEle = document.getElementsByClassName("page1-row");
    //document.getElementsByName
    let len = rowsEle.length;
    for (let index = 0; index < len; index++) {
        const rowEle = rowsEle[index];
        let row = new Object();
        let do_ = rowEle.getElementsByTagName("input")[0].value;
        if (do_ == null || do_ == undefined || do_ == '') {
            alert("外径未输入，将自动忽略");
            continue;
        }
        row.do_ = new Number(do_) / 1000;
        row.thk = new Number(rowEle.getElementsByTagName("input")[1].value) / 1000;
        row.insulThk = new Number(rowEle.getElementsByTagName("input")[2].value) / 1000;
        row.len = new Number(rowEle.getElementsByTagName("input")[3].value);

        let pipe = new Pipe();
        pipe.do_ = row.do_;
        pipe.thk = row.thk;
        pipe.insulThk = row.insulThk;

        row.weight = pipe.weight();
        row.area = pipe.area();

        row.insulVolume = pipe.insulVolume();

        row.kapArea = pipe.kapArea();

        rows.push(row);
    }
    console.log(rows);
    
    
    let table = document.createElement("table");
    table.innerHTML = "<tr><td>外径<br>mm</td><td>单重<br>kg/m</td><td>总重<br>kg</td><td>刷漆面积<br>m2</td><td>保温材料体积<br>m3</td><td>保护层面积<br>m3</td></tr>";
    let weightSum = 0, areaSum = 0, insulSum = 0, kapSum = 0;
    len = rows.length;
    for (let index = 0; index < len; index++) {
        const row = rows[index];
        let tr = document.createElement("tr");
        tr.innerHTML = 
            "<td>" + (row.do_ * 1000).toString() + "</td>"
            + "<td>" + row.weight.toFixed(2) + "</td>"
            + "<td>" + (row.weight * row.len).toFixed(2) + "</td>"
            + "<td>" + (row.area * row.len).toFixed(2) + "</td>"
            + "<td>" + (row.insulVolume * row.len).toFixed(2) + "</td>"
            + "<td>" + (row.kapArea * row.len).toFixed(2) + "</td>";
        table.appendChild(tr);

        weightSum += new Number(row.weight * row.len);
        areaSum += new Number(row.area * row.len);
        insulSum += new Number(row.insulVolume * row.len);
        kapSum += new Number(row.kapArea * row.len);
    }

    let tr = document.createElement("tr");
    tr.innerHTML =
        "<td>合计</td>"
        + "<td> </td>"
        + "<td>" + weightSum.toFixed(2) + "</td>"
        + "<td>" + areaSum.toFixed(2) + "</td>"
        + "<td>" + insulSum.toFixed(2) + "</td>"
        + "<td>" + kapSum.toFixed(2) + "</td>";
    table.appendChild(tr);

    let result = document.getElementById("result");
    result.innerHTML = "";
    result.appendChild(table);

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

function onPage1AddRow() {
    let page1_form = document.getElementById("page1-form");
    //let n = page1_form.children.length;
    let last = page1_form.lastElementChild;
    let nextRowId = 1;
    if (last) {
        let rowStr = last.id;
        let index = rowStr.lastIndexOf('-');
        nextRowId = parseInt(rowStr.slice(index + 1)) + 1;
    } 

    console.log(nextRowId);
    let row = document.createElement("tr");
    row.id = "page1-row-" + nextRowId.toString();
    row.classList.add("page1-row");
    row.classList.add("row");
    let html = `    
            <td><input type="number"></td>
            <td><input type="number"></td>
            <td><input type="number"></td>
            <td><input type="number"></td>
            <td><button onclick="onPage1DelRow(` + nextRowId.toString() + `)">删除</button></td>
        `;
    row.innerHTML = html;
    page1_form.appendChild(row);
}

function  onPage1DelRow(id) {
    document.getElementById("page1-row-" + id.toString()).remove();
}
