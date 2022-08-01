"use strict"

function Router() {
    this.currentUrl = '';
}

Router.prototype.refresh = function () {   
    document.getElementById(this.currentUrl)?.classList.remove("show");
    document.getElementById("page-result").classList.remove("show");
    document.getElementById("report").removeAttribute("href");

    this.currentUrl = location.hash.slice(1) || '/';

    let curPage = document.getElementById(this.currentUrl);
    if (curPage != null) {
        curPage.classList.add("show");
    }
};

Router.prototype.init = function () {
    window.addEventListener('load', this.refresh.bind(this), false);
    window.addEventListener('hashchange', this.refresh.bind(this), false);
};
window.Router = new Router();
window.Router.init();
/*
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
*/
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
    else {
        alert("最后一项只允许修改");
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

window.Runner.method ("page1", function () {
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

    let content = rowEle.parentNode.parentNode.outerHTML;
    console.log(content);
    //responseType: "blob";
    let blob = new Blob([content], {type: "application/msword;charset=utf-8"});
    document.getElementById("report").href = URL.createObjectURL(blob);
});

window.Runner.method ("page2", function () {
    alert("page2 run");
});

//   function downloadFile(file) {
    
//     dwonloadFiles({ fileName: file.name }).then(response => {
//       let blob = new Blob([response]);
//       let downloadElement = document.createElement("a");
//       let href = window.URL.createObjectURL(blob); //创建下载的链接
//       downloadElement.href = href;
//       console.log(file.name, "文件名");
//       downloadElement.download = file.name; //下载后文件名
//       document.body.appendChild(downloadElement);
//       downloadElement.click(); //点击下载
//       document.body.removeChild(downloadElement); //下载完成移除元素
//       window.URL.revokeObjectURL(href); //释放掉blob对象
//     });
 // }
// // 知识链库下载文件
// export function dwonloadFiles(query) {
//     return request({
//       url: '/system/knowledgechain/download',
//       method: 'get',
//       params: query,
//       responseType: 'blob'
//     })
//   }
//   function downloadFile(file) {
//     dwonloadFiles({ fileName: file.name }).then(response => {
//       let blob = new Blob([response]);
//       let downloadElement = document.createElement("a");
//       let href = window.URL.createObjectURL(blob); //创建下载的链接
//       downloadElement.href = href;
//       console.log(file.name, "文件名");
//       downloadElement.download = file.name; //下载后文件名
//       document.body.appendChild(downloadElement);
//       downloadElement.click(); //点击下载
//       document.body.removeChild(downloadElement); //下载完成移除元素
//       window.URL.revokeObjectURL(href); //释放掉blob对象
//     });
//   }
// export function exportExcel(url, params = {}) {
//     return new Promise((resolve, reject) => {
//       axios({
//         method: "get",
//         url: url, // 请求地址
//         params,
//         responseType: "blob" // 表明返回服务器返回的数据类型
//       }).then(
//         (response) => {
//           resolve(response);
//           console.log(response)
//           // 前面responseType设置的 “blob” 后台返回的response直接就是 blob对象，前端不用再去new Blob了
//           // let blob = new Blob([response], {
//           //   type: "application/vnd.ms-excel"
//           // });
//           // console.log(blob);
//           let fileName = "导出单据" + Date.parse(new Date()) + ".xls";
//           if (window.navigator.msSaveOrOpenBlob) {
//             navigator.msSaveBlob(response, fileName);
//           } else {
//             let link = document.createElement("a");
   
//             link.href = window.URL.createObjectURL(response);
//             link.download = fileName;
//             link.click();
//             // 释放内存
//             window.URL.revokeObjectURL(link.href);
   
//           }
//         },
//         (err) => {
//           reject(err);
//         }
//       );
//     });
//   }