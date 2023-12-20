"use strict"
import unitConverter from "./unitConverter.js"

export default class Fm {
    constructor(form, result){
        this._form = [...form];
        this._result = [...result];
    }
    
    getValue(key){
        let d = this._form.concat(this._result);
        for (let i = 0; i < d.length; i++) {
            if (d[i].id == key) {
                if (d[i].type == "number") {
                    return parseFloat(d[i].value);
                } else {
                    return d[i].value;
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
            if (this._form[i].required && !document.getElementById(this._form[i].id).value) {
                //document.getElementById("warning").innerText = this._form[i].name + "必须填写";
                return this._form[i].name;
            } else {
                this._form[i].value = document.getElementById(this._form[i].id).value;
            }
        }
        return 0;
    }
    render(){
        this._result.forEach(item => {
            document.getElementById(item.id).value = item.value;
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
        let w = pipe_weight(d, delta, rho);
        this.setValue("pw", unitConverter("kg/mto" + this.getUnit("pw"), w));
        this.setValue("tw", unitConverter("kgto" + this.getUnit("tw"), w * len));        
    }
    pipeSize() {
        let dn = this.getValue("dn");
        let sch = this.getValue("sch");
        let p = pipe_size(dn, sch);
        this.setValue("d", unitConverter("mmto" + this.getUnit("d"), p.d));
        this.setValue("delta", unitConverter("mmto" + this.getUnit("delta"), p.delta));
        this.setValue("m", unitConverter("kg/mto" + this.getUnit("m"), p.m));
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
    pipeInsultion() {
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
    waterPipe() {
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
    pumpPower() {

    }
}
