"use strict"
import unitConverter from "./unitConverter.js"



let fmData = {
    "text": "管径计算",
    "form": [
        {
            "name": "管线编号",
            "tag": "input",
            "id": "pcode",
            "type": "text",
            "placeholder": "",
            "value": "PI1",
            "required": false,
            "readonly": false,
            "other": "",
            "pclass": "form-group",
            "class": ""
        },{
            "name": "管线名称",
            "tag": "input",
            "id": "pname",
            "type": "text",
            "placeholder": "输入管线名称",
            "value": "",
            "required": false,
            "readonly": false,
            "other": "",
            "pclass": "form-group",
            "class": ""
        },{
            "name": "体积流量",
            "tag": "input",
            "id": "flowRate",
            "type": "number",
            "placeholder": "输入体积流量",
            "value": "",
            "required": true,
            "readonly": false,
            "other": "m3/h",
            "pclass": "form-group",
            "class": ""
        },{
            "name": "流速",
            "tag": "input",
            "id": "v",
            "type": "number",
            "placeholder": "输入流速",
            "value": "",
            "required": true,
            "readonly": false,
            "other": "m/s",
            "pclass": "form-group",
            "class": ""
        }
    ],
    "result": [
        {
            "name": "管线内径",
            "tag": "input",
            "id": "d",
            "type": "text",
            "placeholder": "",
            "value": "",
            "required": false,
            "readonly": true,
            "other": "mm",
            "pclass": "form-group",
            "class": ""
        }
    ]
}

class Fm {
    constructor(text, form, result){
        this.text = text;
        this._form = [...form];
        this._result = [...result];
    }
    getValue(key){
        let d = this._form.concat(this._result);
        for (let i = 0; i < d.length; i++) {
            if (d[i].id == key) {
                if (d[i].type == "number") {
                    return parseFloat(d.value);
                } else {
                    return d.value;
                }
            }
        }
        return false;
    }
    getUnit(key){
        let d = this._form.concat(this._result);
        for (let i = 0; i < d.length; i++) {
            if (d[i].id == key) {
                if (d[i].unit != "") {
                    return d[i].unit;
                } else {
                    return '';
                }
            }
        }
        return '';
    }
    setValue(key, value){
        for (let i = 0; i < this._form.length; i++) {
            if (this._form[i].id == key) {
                this._form[i].value = value;
                return true;
            }
        }

        for (let i = 0; i < this._result.length; i++) {
            if (this._result[i].id == key) {
                this._result[i].value = value;
                return true;
            }
        }
        return false;
    }
    fillFormValue(){
        for (let i = 0; i < this._form.length; i++) {
            if (this._form[i].required && !document.getElementById(item.id).value) {
                document.getElementById("waring").innerText = item.name + "必须填写";
                return false;
            } else {
                this._form[i].value = document.getElementById(item.id).value;
            }
        }
        return true;
    }
    render(){
        this._result.forEach(item => {
            document.getElementById(item.id).value = value;
        });
    }
    pipeDiameter() {
        let flowRate = unitConverter(this.getUnit("flowRate") + "tom3/s", this.getValue("flowRate"));
        let v = unitConverter(this.getUnit("v") + "tom/s", this.getValue("v"));
        this.setValue("di", unitConverter('mto' + this.getUnit("di"), pipe_diameter(flowRate, v)));    
    }
    pipeVelocity() {
        let flowRate = unitConverter(this.getUnit("flowRate") + "tom3/s", this.getValue("flowRate"));
        let di = unitConverter(this.getUnit("di") + "tom", this.getValue("di") );
        this.setValue("v", unitConverter('m/s' + this.getUnit("v"), pipe_velocity(flowRate, di)));
    }
    pipeWeight() {
        let d = unitConverter(this.getUnit("d") + "tom", this.getValue("d"));
        let delta = unitConverter(this.getUnit("delta") + "tom", this.getValue("delta"));
        let len = unitConverter(this.getUnit("len") + "tom", this.getValue("len"));
        let rho = unitConverter(this.getUnit("rho") + "tokg/m3", this.getValue("rho"));
        let w = pipe_weight(d, delta, len, rho);
        this.setValue("pw", unitConverter("kg/mto" + this.getUnit("pw"), w.pw));
        this.setValue("tw", unitConverter("kg/to" + this.getUnit("tw"), w.tw));        
    }
    function pipeStrength(fmData) {
        let d = unitConverter(fmData.form[2].other + "tomm", fmData.form[2].value);
        let p = unitConverter(fmData.form[3].other + "toMPa", fmData.form[3].value);
        let s = unitConverter(fmData.form[4].other + "toMPa", fmData.form[4].value);
        let y = fmData.form[5].value;
        let w = fmData.form[6].value;
        let phi = fmData.form[7].value;
        let c1 = fmData.form[8].value;
        let c2 = unitConverter(fmData.form[9].other + "tomm", fmData.form[9].value);
        let c3 = unitConverter(fmData.form[10].other + "tomm", fmData.form[10].value);
        let r = unitConverter(fmData.form[11].other + "tomm", fmData.form[11].value);
    
        let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
        let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);
    
        for (let i = 0; i < l.length; i++) {
            fmData.result[i].value = unitConverter('mm' + fmData.result[i].other, l[i]).toFixed(2);
        }
        for (let i = 0; i < b.length; i++) {
            fmData.result[i + l.length].value = unitConverter('mm' + fmData.result[ + l.length].other, b[i]).toFixed(2);
        }
    }
    function pipeInsultion(fmData) {
        let d0 = unitConverter(fmData.form[2].other + "tom", fmData.form[2].value);
        let t0 = unitConverter(fmData.form[3].other + "toC", fmData.form[3].value);
        let ta = unitConverter(fmData.form[4].other + "toC", fmData.form[4].value);
        let w = unitConverter(fmData.form[5].other + "tom/s", fmData.form[5].value);
        let delta = unitConverter(fmData.form[6].other + "tom", fmData.form[6].value);
        let d1 = d0 + 2 * delta;
        let epsilon = fmData.form[7].value;
        let lambdaArgs = []
        for (let i = 0; index < 7; i++) {
            lambdaArgs.push(fmData.form[i + 8].value);
        }
    
        let insul = pipe_insultion(t0, ta, w, d0, d1, lambdaArgs, epsilon);
        
        for (let i = 0; index < insul.length; i++) {
            fmData.result[i].value = insul[i].toFixed(2);
        }
    }
    function waterPipe(fmData) {
        let flowRate = unitConverter(fmData.form[2].other + "tokg/s", fmData.form[2].value);
        let a_t = unitConverter(fmData.form[3].other + "toC", fmData.form[3].value);
        let a_p = unitConverter(fmData.form[4].other + "toMPaA", fmData.form[4].value);
        let d0 = unitConverter(fmData.form[2].other + "tom", fmData.form[2].value);
        let t0 = unitConverter(fmData.form[3].other + "toC", fmData.form[3].value);
        let ta = unitConverter(fmData.form[4].other + "toC", fmData.form[4].value);
        let w = unitConverter(fmData.form[5].other + "tom/s", fmData.form[5].value);
        let delta = unitConverter(fmData.form[6].other + "tom", fmData.form[6].value);
        let d1 = d0 + 2 * delta;
        let epsilon = fmData.form[7].value;
        let lambdaArgs = []
        for (let i = 0; index < 7; i++) {
            lambdaArgs.push(fmData.form[i + 8].value);
        }
    
        let insul = pipe_insultion(t0, ta, w, d0, d1, lambdaArgs, epsilon);
        
        for (let i = 0; index < insul.length; i++) {
            fmData.result[i].value = insul[i].toFixed(2);
        }
    }
}
