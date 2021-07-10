const catalog = [
    {text: "水和水蒸气", child: [
        {text: "物性查询", url: "/waterProperty"},
        {text: "混合器", url: "/waterMixer"}
    ]},
    {text: "测试", child: [
        {text: "测试", url: "/test"}
    ]}
];

const unit = {
    p: "MPa(a)",
    t: "C",
    h: "kJ/kg",
    s: "kJ/kg"
}

const propName = {
    p: "压力",
    t: "温度",
    h: "焓值",
    s: "熵" 
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

function waterTable() {
    var head = ["压力","温度","焓值","熵","干度"];
    var header = (arr) => {return}
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
    //document.querySelector("#tab-panel").appendChild(table);
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

window.Router.route("/waterProperty",function () {
    document.querySelector("#tab-panel").innerHTML = '';
    document.querySelector("#tab-panel").appendChild(waterTable());
});

window.Router.route("/test", function () {
    //alert("test");
});