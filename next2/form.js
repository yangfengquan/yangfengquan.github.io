// const fm = window.location.hash.substring(1)
// var curFormDataObj = {}
// const formDataPath = 'data.json'
// const xhr = new XMLHttpRequest()
// xhr.open('GET', formDataPath, true)
// xhr.send()
// xhr.onreadystatechange = function() {
//     if (xhr.readyState === 4 && xhr.status === 200) {
//         const response = JSON.parse(xhr.responseText)
//         curFormDataObj = response[fm]
//         document.getElementById('form-title').innerText = curFormDataObj.text
//         let formEle = document.getElementById("form")
//         form(curFormDataObj.form, formEle)
//         let resultEle = document.getElementById('result')
//         form(curFormDataObj.result, resultEle)
//     }
// }

function form(formdata, formEle) {
    //let formEle = document.getElementById("form")
    formdata.forEach(item => {
        switch (item.tag) {
            case 'input':
                formEle.appendChild(
                    createInputEle(
                        item.name, item.id, item.type, item.placeholder,
                        item.value, item.required, item.readonly, item.other))
                break;
            case 'select':
                formEle.appendChild(
                    createSelectEle(item.name, item.id, item.opts, item.value,
                        item.required, item.readonly, item.listener, item.other))
            default:
                break;
        }
    })   
}
function createInputEle(name, id, type, placeholder, value, required, readonly,other) {
    let groupEle = document.createElement('p')
    groupEle.classList.add(id)
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
    groupEle.appendChild(inputEle)

    if (other != '') {
        let spanEle = document.createElement('span')
        spanEle.innerText = other
        groupEle.appendChild(spanEle)
    }
    
    return groupEle
}

function createSelectEle(name, id, opts, value, required, readonly, listener, other) {
    let groupEle = document.createElement('p')

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
    curFormDataObj.form.forEach(item => {
        let value = document.getElementById(item.id).value
        document.getElementById('warning').innerText = ''
        try {
            if (value == '' && item.required) {
                throw item.name + '不可为空！'
            }
            item.value = value
        } catch (error) {
            document.getElementById('warning').innerText = error
        }
    });
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