function addFavorite(title,url){
    try{
        window.external.addFavorite(url,title);
    }
    catch(e){
        try{
            window.sidebar.addPanel(title,url,"");
        }
        catch(e){
            alert("抱歉，您所使用的浏览器无法完成此操作。\n\n请使用快捷键Ctrl+D进行添加！");
        }
    }
}

function createInput(data) {
    var p = document.createElement("p");
    
    var label = document.createElement("label");
    label.innerText = (data.label || propName[data.name] || '') + "\n" + (data.unit || unit[data.name] || '');
    p.appendChild(label);

    var input = document.createElement("input");
    input.name = data.name || '';
    input.type = data.type || "text";
    input.readOnly = data.readOnly || false;
    input.required = data.required || false;
    input.placeholder = data.placeholder || '';
    input.value = data.value || '';
    input.autocomplete = data.autocomplete || "on";
    p.appendChild(input);

    return p;
}

function createSelect(data, callback) {
    var p = document.createElement("p");
    
    var label = document.createElement("label");
    label.innerText = (data.label || propName[data.name] || '') + "\n" + (data.unit || unit[data.name] || '');
    p.appendChild(label);

    var select = document.createElement("select");
    select.name = data.name || '';
    select.required = data.required || false;
    data.option.forEach((el, index) => {
        var option = document.createElement("option");
        option.innerText = Array.isArray(el) ? el[0] : el;
        option.value = Array.isArray(el) ? el[1] : index;
        select.appendChild(option);
    });
    select.addEventListener("change", callback);
    p.appendChild(select);

    return p;
}

function createButton(text, callback) {
    var btn = document.createElement("button");
    btn.innerText = text;
    btn.addEventListener("click", callback);
    return btn;
}

function createRes(res) {
    var p = document.createElement("p");
    var label = document.createElement("label");
    label.innerText = (propName[res.name] || '') + "\n" + (unit[res.name] || '');
    p.appendChild(label);
    var value = document.createElement("span");
    value.innerText = res.value; 
    p.appendChild(value);
    return p;
}

