const form = {};
form.list = {};
form.pipeDiameter     = {"input": ["volume_flowrate", "ve"], "out":["inside_diameter"]}
form.volume_flowrate  = {"text": "体积流量",     "tag": "input",  "type": "number",  "unitset": "volumeFlowRate_unit" };
form.ve               = {"text": "流速",         "tag": "input",  "type": "number",  "unitset": "speed_unit"          };
form.inside_diameter  = {"text": "圆管内径",      "tag": "input",  "type": "number",  "unitset": "length_unit"        };

function createInputGroupEle(url, id) {
    let groupEle = document.createElement('div');
    groupEle.className = "form-group";
    groupEle.id = url + "-" + id + "-form-group";

    let labelEle = document.createElement('label');
    labelEle.innerText = form[id].text;
    labelEle.setAttribute('for', url + "-" + id);
    groupEle.appendChild(labelEle);

    let inputEle = document.createElement('input');
    inputEle.setAttribute('name', url + "-" + id);
    inputEle.setAttribute('id', url + "-" + id);
    inputEle.type = form[id].type;
    inputEle.placeholder = "请输入";
    groupEle.appendChild(inputEle)
   
    if (form[id].hasOwnProperty("unitset") && form[id].unitset != '') {
        //groupEle.appendChild(createUnitSelect(inp.id, inp.unitset, inp.unit));
    }

    return groupEle
}
