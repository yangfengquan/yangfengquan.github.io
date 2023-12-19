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
    pipeStrength() {
        let d = unitConverter(this.getUnit("d") + "tomm", this.getValue("d"));
        let p = unitConverter(this.getUnit("p") + "toMPa", this.getValue("p"));
        let s = unitConverter(this.getUnit("s") + "toMPa", this.getValue("s"));
        let y = this.getValue("y");
        let w = this.getValue("w");
        let phi = this.getValue("phi");
        let c1 = this.getValue("c1");
        let c2 = unitConverter(this.getUnit("c2") + "tomm", this.getValue("c2"));
        let c3 = unitConverter(this.getUnit("c3") + "tomm", this.getValue("c3"));
        let r = unitConverter(this.getUnit("r") + "tomm", this.getValue("r"));
    
        let l = pipe_strength(d, p, s, y, w, phi, c1, c2, c3);
        let b = bend_strength(d, p, s, y, w, phi, c1, c2, c3, r);
    
        this.setValue("t", unitConverter("mto" + this.getUnit("t"), l.t));
        this.setValue("nt", unitConverter("mto" + this.getUnit("nt"), l.nt));
        this.setValue("t1", unitConverter("mto" + this.getUnit("t1"), b.t1));
        this.setValue("nt1", unitConverter("mto" + this.getUnit("nt1"), b.nt1));
        this.setValue("t2", unitConverter("mto" + this.getUnit("t2"), b.t2));
        this.setValue("nt2", unitConverter("mto" + this.getUnit("nt2"), b.nt2));
        this.setValue("t3", unitConverter("mto" + this.getUnit("t3"), b.t3));
        this.setValue("nt3", unitConverter("mto" + this.getUnit("nt3"), b.nt3));
    }
    pipeInsultion(fmData) {
        let d0 = unitConverter(this.getUnit("d0") + "tom", this.getValue("d0"));
        let t0 = unitConverter(this.getUnit("t0") + "toC", this.getValue("t0"));
        let ta = unitConverter(this.getUnit("ta") + "toC", this.getValue("ta"));
        let w = unitConverter(this.getUnit("w") + "tom/s", this.getValue("w"));
        let delta = unitConverter(this.getUnit("delta") + "tom", this.getValue("delta"));
        let d1 = d0 + 2 * delta;
        let epsilon = this.getValue("epsilon");
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));

        let insul = pipe_insultion(t0, ta, w, d0, d1, lambdaArgs, epsilon);
        
        this.setValue("lambda", unitConverter('W/(m*K)to' + this.getUnit("lambda"), insul.lambda));
        this.setValue("alpha", unitConverter('W/(m2*K)to' + this.getUnit("alpha"), insul.alpha));
        this.setValue("ts", unitConverter('Cto' + this.getUnit("ts"), insul.ts));
        this.setValue("Q", unitConverter('W/m2to' + this.getUnit("Q"), insul.Q));
        this.setValue("q", unitConverter('W/mto' + this.getUnit("q"), insul.q));
    }
    waterPipe(fmData) {
        let flowRate = unitConverter(this.getUnit("flowRate") + "tokg/s", this.getValue("flowRate"));
        let a_t = unitConverter(this.getUnit("a_t") + "toC", this.getValue("a_t"));
        let a_p = unitConverter(this.getUnit("a_p") + "toMPaA", this.getValue("a_p"));
        let d0 = unitConverter(this.getUnit("d0") + "tom", this.getValue("d0"));
        let delta = unitConverter(this.getUnit("delta") + "tom", this.getValue("delta"));
        let ll = unitConverter(this.getUnit("ll") + "tom", this.getValue("ll"));
        let lf = unitConverter(this.getUnit("lf") + "tom", this.getValue("lf"));
        let rough = unitConverter(this.getUnit("rough") + "tom", this.getValue("rough"));
        let insuldelta = unitConverter(this.getUnit("insuldelta") + "tom", this.getValue("insuldelta"));
        
        let lambdaArgs = [];
        lambdaArgs.push(this.getValue("a"));
        lambdaArgs.push(this.getValue("b"));
        lambdaArgs.push(this.getValue("c"));
        lambdaArgs.push(this.getValue("d"));
        lambdaArgs.push(this.getValue("e"));
        lambdaArgs.push(this.getValue("f"));
        lambdaArgs.push(this.getValue("g"));
        
        let epsilon = this.getValue("epsilon");

        let ta = unitConverter(this.getUnit("ta") + "toC", this.getValue("ta"));
        let w = unitConverter(this.getUnit("w") + "tom/s", this.getValue("w"));
        
        let di = d0 - 2 * delta;
        let d1 = d0 + 2 * insuldelta;
        let wp = water_pipe(flowRate, a_t, a_p, di, d0, d1, ll, lf, rough, lambdaArgs, epsilon, ta, w);
        
        for (const key in wp) {
            if (Object.hasOwnProperty.call(wp, key)) {
                if (this.getUnit(key) == "MPa") {
                    this.setValue(unitConverter("MPaAto" + this.getUnit(key), wp[key]));
                } else {
                    this.setValue(unitConverter(this.getUnit(key) + "to" + this.getUnit(key), wp[key]));
                }
            }
        }
    }
}
