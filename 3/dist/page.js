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
        row.length = new Number(rowEle.getElementsByTagName("input")[3].value);

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
    
    let table = document.createElement("table");
    table.innerHTML = "<tr><td>外径<br>mm</td><td>单重<br>kg/m</td><td>总重<br>kg</td><td>刷漆面积<br>m2</td><td>保温材料体积<br>m3</td><td>保护层面积<br>m3</td></tr>";
    let weightSum = 0, areaSum = 0, insulSum = 0, kapSum = 0;

    for (let index = 0; index < rows.length; index++) {
        const row = rows[index];
        let tr = document.createElement("tr");
        tr.innerHTML = 
            "  <td>" + (row.do_ * 1000).toString() + "</td>"
            + "<td>" + row.weight.toFixed(2) + "</td>"
            + "<td>" + (row.weight * row.length).toFixed(2) + "</td>"
            + "<td>" + (row.area * row.length).toFixed(2) + "</td>"
            + "<td>" + (row.insulVolume * row.length).toFixed(2) + "</td>"
            + "<td>" + (row.kapArea * row.length).toFixed(2) + "</td>";
        table.appendChild(tr);

        weightSum += new Number(row.weight * row.length);
        areaSum += new Number(row.area * row.length);
        insulSum += new Number(row.insulVolume * row.length);
        kapSum += new Number(row.kapArea * row.length);
    }

    let tr = document.createElement("tr");
    tr.innerHTML =
        "  <td>合计</td>"
        + "<td>&nbsp</td>"
        + "<td>" + weightSum.toFixed(2) + "</td>"
        + "<td>" + areaSum.toFixed(2) + "</td>"
        + "<td>" + insulSum.toFixed(2) + "</td>"
        + "<td>" + kapSum.toFixed(2) + "</td>";
    table.appendChild(tr);

    let result = document.getElementById("result");
    result.innerHTML = "";
    result.appendChild(table);

    document.getElementById("page-result").classList.add("show");

    let text = "外径\t\t壁厚\t\t保温厚\t管长\t\t单重\t\t总重\t\t刷漆面积\t保温材料体积\t保护层面积\nmm\t\tmm\t\tmm\t\tm\t\tkg/m\t\tkg\t\tm2\t\tm3\t\t\tm2\n";
    for (let index = 0; index < rows.length; index++) {
        const row = rows[index];
        text += (row.do_ * 1000).toString() + "\t\t";
        text += (row.thk * 1000).toString() + "\t\t";
        text += (row.insulThk * 1000).toString() + "\t\t";
        text += row.length.toString() + "\t\t";
        text += row.weight .toFixed(2) + "\t\t";
        text += (row.weight * row.length).toFixed(2) + "\t\t";
        text += (row.area * row.length).toFixed(2) + "\t\t";
        text += (row.insulVolume * row.length).toFixed(2) + "\t\t\t";
        text += (row.kapArea * row.length).toFixed(2) + "\n";
    }

    toReport(text);
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
        console.log(jsonObj);
        document.getElementById(window.Router.currentUrl).classList.remove("show");
        
        location.hash = "#" + jsonObj.page;

        let pageEle = document.getElementById(jsonObj.page);
        if (pageEle) {
            let formEle = pageEle.getElementById("form")
            let rows = formEle.getElementsByClassName("row");
            let inputs = rows[0].getElementsByTagName("input");
            for (let i = 0; i < jsonObj.data.length; i++) {
                inputs[i].value = jsonObj.data[i]; 
            }
        }
        
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

function save() {
    let currentPage = document.getElementsByClassName("show")[0];
    //let page = currentPage.id;
    let rowsEle = currentPage.getElementsByClassName("row");

    let rows = new Array();
    for (let i = 0; i < rowsEle.length; i++) {
        const rowEle = rowsEle[i];
        let inputs = rowEle.getElementsByTagName("input");
        let row = new Array();
        for (let j = 0; j < inputs.length; j++) {
            const input = inputs[j];
            row.push(input.value);
        }
        rows.push(row);
    }

    let dataObj = new Object();
    dataObj.page = currentPage.id;
    dataObj.data = rows;
    let jsonStr = JSON.stringify(dataObj);

    let link = document.getElementById("save-link");
    link.href = downLoadLink(jsonStr);
    link.click();
}

function toReport(text) {
    var blob = new Blob([text], { type: "text/plain;charset=utf-8" ,endings:'native'});
    var link = document.getElementById("report");
    link.href = URL.createObjectURL(blob);
}

function downLoadLink(text) {
    var blob = new Blob([text], { type: "text/plain;charset=utf-8" ,endings:'native'});
    return URL.createObjectURL(blob);
}