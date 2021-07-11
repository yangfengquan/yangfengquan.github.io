const catalog = [
    {text: "管子", child: [
        {text: "管径计算(流速)", url: "/dim_v"},
        {text: "管径计算(压降)", url: "/dim_dP"}
    ]},
    {text: "水蒸气", child: [
        {text: "物性", url: "/waterProp"}
    ]}
];

const unit = {
    f: "m3/h",
    ve: "m/s",
    dim: "mm",
    p: "MPa(a)",
    t: "C",
    h: "kJ/kg",
    s: "kJ/kg"
}

const propName = {
    f: "流量",
    ve: "流速",
    dim: "管子内径",
    p: "压力",
    t: "温度",
    h: "焓值",
    s: "熵",
    x: "干度 "
}
function createInput(data, callback) {
    var input = document.createElement("input");
    input.name = data.name || '';
    input.id = data.id || '';
    input.type = data.type || "text";
    input.readOnly = data.readOnly || false;
    input.required = data.required || false;
    input.placeholder = data.placeholder || '';
    input.value = data.value || '';
    input.addEventListener = callback || function(){};

    return input;
}

function createSelect(data, callback) {
    var select = document.createElement("select");
    select.name = data.name;
    select.id = data.id;
    select.required = data.required;
    data.option.forEach((el) => {
        var option = document.createElement("option");
        option.innerText = option[0];
        option.value = option[1];
        select.appendChild(option);
    });

    return select;
}

function createTableHeader(table, head) {
    var thead = table.createTHead();
    var row = thead.insertRow(0);
    head.forEach((el) => {
        var th = document.createElement("th");
        th.innerText = el;
        row.appendChild(th);
    });
}

function addTableRow(table, rowdata) {
    
}

function dim_vTable() {
    var cells = ["f","ve"];
    
    var head = [];
    cells.forEach((el) => {
        head.push((propName[el] || '') + "\n" + (unit[el] || ''));
    });

    var table = document.createElement("table");
    createTableHeader(table, head);
    var tbody = table.createTBody()
    var row = tbody.insertRow();
    cells.forEach((el) => {
        var cell = row.insertCell();
        cell.appendChild(createInput({name: el}));
    })
    return table;
}
function waterTable() {
    var head = [];
    
    ["p","t","h","s","x"].forEach((el) => {
        //head.push((propName.hasOwnProperty(el) ? propName[el] : '') + "\n" + (unit.hasOwnProperty(el) ? unit[el] : ''));
        head.push((propName[el] || '') + "\n" + (unit[el] || ''));
    });
    
    var cells = ["p","t","h","s","x"];
    var table = document.createElement("table");
    createTableHeader(table, head);
    var tbody = table.createTBody()
    var row = tbody.insertRow();
    cells.forEach((el) => {
        var cell = row.insertCell();
        cell.appendChild(createInput({name: el}));
    })
    return table;
}

function initCatalog(data) {
    var ul = document.createElement("ul");
    
    data.forEach(plist => {
       
        var ol = document.createElement("ol");
        plist.child.forEach(clist => {
            var a = document.createElement("a");
            a.textContent = clist.text;
            a.href = '#' + clist.url;
            var li = document.createElement("li");
            li.appendChild(a);
            ol.appendChild(li);
        });
        
        var li = document.createElement("li");
        li.innerText = plist.text;
        li.appendChild(ol);
        ul.appendChild(li);
    });
    
    return ul;
}

var home = function () {
    document.getElementById("catalog").innerHTML = '';
    document.getElementById("catalog").appendChild(initCatalog(catalog));
};

home();

window.Router.route("/", function () {
    home();
});

window.Router.route("/dim_v",function () {
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(dim_vTable());
});

window.Router.route("/waterProp",function () {
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(waterTable());
});

window.Router.route("/test", function () {
    //alert("test");
    document.querySelector("#tab-panel").innerHTML = '';
});