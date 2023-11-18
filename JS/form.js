function form(formdata, formEle) {
    //let formEle = document.getElementById("form")
    formdata.forEach(item => {
        switch (item.tag) {
            case 'input':
                formEle.appendChild(
                    createInputEle(
                        item.name, item.id, item.type, item.placeholder,
                        item.value, item.required, item.readonly, item.other,
                        item.pclass, item.class))
                break;
            case 'select':
                formEle.appendChild(
                    createSelectEle(item.name, item.id, item.opts, item.value,
                        item.required, item.readonly, item.listener, item.other,
                        item.pclass, item.class))
            default:
                break;
        }
    })   
}
function createInputEle(name, id, type, placeholder, value, required,
    readonly,other, pclass, iclass) {
    let groupEle = document.createElement('div')
    //groupEle.classList.add(id)
    groupEle.className = pclass
    let labelEle = document.createElement('label')
    labelEle.innerText = name
    labelEle.setAttribute('for',id)
    groupEle.appendChild(labelEle)

    let inputEle = document.createElement('input')
    inputEle.setAttribute('name', name)
    inputEle.setAttribute('id', id)
    inputEle.setAttribute('autocomplete', 'on')
    inputEle.type = type
    inputEle.value = value
    inputEle.placeholder = placeholder
    inputEle.required = required
    inputEle.readOnly = readonly
    inputEle.className = iclass
    groupEle.appendChild(inputEle)

    if (other != '') {
        let spanEle = document.createElement('span')
        spanEle.innerText = other
        groupEle.appendChild(spanEle)
    }
    
    return groupEle
}

function createSelectEle(name, id, opts, value, required, readonly, listener, other, pclass) {
    let groupEle = document.createElement('div')
    groupEle.className = pclass
    let labelEle = document.createElement('label')
    labelEle.innerText = name
    labelEle.setAttribute('for',id)
    groupEle.appendChild(labelEle)

    let selectEle = document.createElement('select')
    selectEle.setAttribute('name', name)
    selectEle.setAttribute('id', id)
    selectEle.setAttribute('autocomplete', 'on')
    selectEle.required = required
    selectEle.readOnly = readonly
    //selectEle.onchange = listener
    selectEle.setAttribute('onchange', listener)
    opts.forEach(opt => {
        selectEle.add(new Option(opt.text, opt.value))
    });
    selectEle.value = value
    groupEle.appendChild(selectEle)

    if (other != '') {
        let spanEle = document.createElement('span')
        spanEle.innerText = other
        groupEle.appendChild(spanEle)
    }
    
    return groupEle
}

function getFormValue() {
    for (let index = 0; index < curFormDataObj.form.length; index++) {
        curFormDataObj.form[index].value = document.getElementById(curFormDataObj.form[index].id).value
        document.getElementById('warning').innerText = ''
        if (curFormDataObj.form[index].value == '' && curFormDataObj.form[index].required) {
            document.getElementById('warning').innerText = curFormDataObj.form[index].name + '不可为空！'
            return false
        }
        
    }
    return true
}

var propObj = {
    T:   {name: "温度",         unit: "C",        },
    p:   {name: "压力",         unit: "MPaA",     },
    h:   {name: "比焓",         unit: "kJ/kg",    },
    v:   {name: "比体积",       unit: "m3/kg",    },
    rho: {name: "密度",         unit: "kg/m3",    },
    s:   {name: "比熵",         unit: "kJ/kg",    },
    u:   {name: "内能",         unit: "kJ/kg",    },
    Cp:  {name: "定压比热容",   unit: "kJ/(kg·K)",},
    Cv:  {name: "定容比热容",   unit: "kJ/(kg·K)",},
    w:   {name: "声速",         unit: "m/s",      },
    my:  {name: "动力粘度",     unit: "Pa·s",     },
    tc:  {name: "导热系数",     unit: "W/(m·K)",  },
    st:  {name: "表面张力",     unit: "N/m",      },
    x:   {name: "干度",         unit: "",         },
    vx:  {name: "气态体积分数", unit: "",         }
}

var modes = [
        ['p','T'],
        ['p','h'],
        ['p','s'],
        ['p','rho'],
        ['p','x'],
        ['T','x'],
        ['h','s'],
        ['h','rho']
    ]
function onWaterArg1Change() {
    let arg1 = document.getElementById('waterArg1').value;

    let arg2_ele = document.getElementById("waterArg2");
    arg2_ele.innerHTML = "";
    modes.forEach(item => {
        if (item[0] == arg1) {
            arg2_ele.options.add(new Option(propObj[item[1]].name, item[1]));
        }
    })
    document.getElementsByClassName('waterValue1')[0].lastChild.innerHTML = propObj[arg1].unit;
    document.getElementsByClassName('waterValue2')[0].lastChild.innerHTML = propObj[arg2_ele.value].unit;
}
function onWaterArg2Change() {
    let arg2_ele = document.getElementById("waterArg2");
    document.getElementsByClassName('waterValue2')[0].lastChild.innerHTML = propObj[arg2_ele.value].unit;
}