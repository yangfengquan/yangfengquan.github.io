const catalog = [
    {text: "水和水蒸气", child: [
        {text: "物性查询", url: "/waterProperty"},
        {text: "混合器", url: "/waterMixer"}
    ]},
    {text: "测试", child: [
        {text: "测试", url: "/test"}
    ]}
];

const content = {
    first: [
        {text: "压力", subtext: "MPa", type: "number", el: "input", required: true}
    ]
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
        
        var a = document.createElement("a");
        a.textContent = plist.text;
        a.href = "javascript:;"
        var li = document.createElement("li");
        li.appendChild(a);
        li.appendChild(ol);
        ul.appendChild(li);
    });
    
    return ul;
}

var home = function () {
    document.getElementById("catalog").innerHTML = '';
    document.getElementById("catalog").appendChild(initCatalog(catalog));
}();

window.Router.route("/", function () {
    home();
});

window.Router.route("/test", function () {
    //alert("test");
});