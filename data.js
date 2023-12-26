const db =
{
    "pipeDiameter": {
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
                "unit": "m3/h",
                "unitset": "volumeFlowRate_unit",
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
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "管线内径",
                "tag": "input",
                "id": "di",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeVelocity": {
        "text": "管道流速计算",
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
                "unit": "m3/h",
                "unitset": "volumeFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管线内径",
                "tag": "input",
                "id": "di",
                "type": "number",
                "placeholder": "输入管线内径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "流速",
                "tag": "input",
                "id": "v",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeWeight": {
        "text": "管子重量计算",
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
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子外径",
                "tag": "input",
                "id": "d",
                "type": "number",
                "placeholder": "输入管子外径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子壁厚",
                "tag": "input",
                "id": "delta",
                "type": "number",
                "placeholder": "输入管子壁厚",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管材密度",
                "tag": "input",
                "id": "rho",
                "type": "number",
                "placeholder": "",
                "value": "7850",
                "required": true,
                "readonly": false,
                "unit": "kg/m3",
                "unitset": "density_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子长度",
                "tag": "input",
                "id": "len",
                "type": "number",
                "placeholder": "输入管子长度",
                "value": "",
                "required": false,
                "readonly": false,
                "unit": "m",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "单重",
                "tag": "input",
                "id": "pw",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "总重",
                "tag": "input",
                "id": "tw",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg",
                "unitset": "mass_unit",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeSize": {
        "text": "管子尺寸查询[SH/T3405]",
        "form": [
            {
                "name": "公称直径",
                "tag": "select",
                "id": "dn",
                "value": "",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "value": "DN6",
                        "text": "DN6"
                    },{
                        "value": "DN8", 
                        "text": "DN8"
                    },{
                        "value": "DN10", 
                        "text": "DN10"
                    },{
                        "value": "DN15", 
                        "text": "DN15"
                    },{
                        "value": "DN20", 
                        "text": "DN20"
                    },{
                        "value": "DN25", 
                        "text": "DN25"
                    },{
                        "value": "DN32", 
                        "text": "DN32"
                    },{
                        "value": "DN40", 
                        "text": "DN40"
                    },{
                        "value": "DN50", 
                        "text": "DN50"
                    },{
                        "value": "DN65", 
                        "text": "DN65"
                    },{
                        "value": "DN80", 
                        "text": "DN80"
                    },{
                        "value": "DN90", 
                        "text": "DN90"
                    },{
                        "value": "DN100", 
                        "text": "DN100"
                    },{
                        "value": "DN125", 
                        "text": "DN125"
                    },{
                        "value": "DN150", 
                        "text": "DN150"
                    },{
                        "value": "DN200", 
                        "text": "DN200"
                    },{
                        "value": "DN250", 
                        "text": "DN250"
                    },{
                        "value": "DN300", 
                        "text": "DN300"
                    },{
                        "value": "DN350", 
                        "text": "DN350"
                    },{
                        "value": "DN400", 
                        "text": "DN400"
                    },{
                        "value": "DN450", 
                        "text": "DN450"
                    },{
                        "value": "DN500", 
                        "text": "DN500"
                    },{
                        "value": "DN550", 
                        "text": "DN550"
                    },{
                        "value": "DN600", 
                        "text": "DN600"
                    }
                ],
                "pclass": "form-group",
                "class": ""
            },{
                "name": "壁厚系列",
                "tag": "select",
                "id": "sch",
                "value": "",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "value": "SCH5",
                        "text": "SCH5"
                    },{
                        "value": "SCH10", 
                        "text": "SCH10"
                    },{
                        "value": "SCH20", 
                        "text": "SCH20"
                    },{
                        "value": "SCH30", 
                        "text": "SCH30"
                    },{
                        "value": "SCH40", 
                        "text": "SCH40"
                    },{
                        "value": "SCH60", 
                        "text": "SCH60"
                    },{
                        "value": "SCH80", 
                        "text": "SCH80"
                    },{
                        "value": "SCH100", 
                        "text": "SCH100"
                    },{
                        "value": "SCH120", 
                        "text": "SCH120"
                    },{
                        "value": "SCH140", 
                        "text": "SCH140"
                    },{
                        "value": "SCH160", 
                        "text": "SCH160"
                    },{
                        "value": "STD", 
                        "text": "STD"
                    },{
                        "value": "XS", 
                        "text": "XS"
                    },{
                        "value": "XXS", 
                        "text": "XXS"
                    },{
                        "value": "SCH5S", 
                        "text": "SCH5S"
                    },{
                        "value": "SCH10S", 
                        "text": "SCH10S"
                    },{
                        "value": "SCH40S", 
                        "text": "SCH40S"
                    },{
                        "value": "SCH80S", 
                        "text": "SCH80S"
                    }
                ],
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "外径",
                "tag": "input",
                "id": "d",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "壁厚",
                "tag": "input",
                "id": "delta",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm"
            },{
                "name": "单重",
                "tag": "input",
                "id": "m",
                "type": "text",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeStrength": {
        "text": "管子强度[壁厚]计算",
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
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子外径",
                "tag": "input",
                "id": "d",
                "type": "number",
                "placeholder": "输入管子外径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "设计压力",
                "tag": "input",
                "id": "p",
                "type": "number",
                "placeholder": "输入设计压力",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "MPa",
                "unitset": "pressure_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "许用应力",
                "tag": "input",
                "id": "s",
                "type": "number",
                "placeholder": "查GB/T20801.2 表A.1",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "MPa",
                "unitset": "pressure_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "计算系数",
                "tag": "input",
                "id": "y",
                "type": "number",
                "placeholder": "查GB/T20801.2 6.1款 计算系数Y",
                "value": "0.4",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "焊接接头高温强度降低系数",
                "tag": "input",
                "id": "w",
                "type": "number",
                "placeholder": "查GB/T20801.3 4.2.7款焊接接头高温强度降低系数W",
                "value": "1",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "纵向焊接接头系数",
                "tag": "input",
                "id": "phi",
                "type": "number",
                "placeholder": "查GB/T20801.2 表A.3、表A.4",
                "value": "1",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "材料厚度负偏差",
                "tag": "input",
                "id": "c1",
                "type": "number",
                "placeholder": "查按管子标准",
                "value": "0.125",
                "required": true,
                "readonly": false,
                "other": "%",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "腐蚀、冲蚀裕量",
                "tag": "input",
                "id": "c2",
                "type": "number",
                "placeholder": "",
                "value": "1.5",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "机械加工深度",
                "tag": "input",
                "id": "c3",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管弯曲半径",
                "tag": "input",
                "id": "r",
                "type": "number",
                "placeholder": "输入管子中心处的弯曲半径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "直管计算厚度",
                "tag": "input",
                "id": "t",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "直管名义厚度",
                "tag": "input",
                "id": "nt",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管内侧计算厚度",
                "tag": "input",
                "id": "t1",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管内侧名义厚度",
                "tag": "input",
                "id": "nt1",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管外侧计算厚度",
                "tag": "input",
                "id": "t2",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管外侧名义厚度",
                "tag": "input",
                "id": "nt2",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管中心线处侧壁计算厚度",
                "tag": "input",
                "id": "t3",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "弯管中心线处侧壁名义厚度",
                "tag": "input",
                "id": "nt3",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeHInsultion": {
        "text": "管道保温计算",
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
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子外径",
                "tag": "input",
                "id": "d0",
                "type": "number",
                "placeholder": "输入管子外径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "物料温度",
                "tag": "input",
                "id": "t0",
                "type": "number",
                "placeholder": "输入物料温度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "环境温度",
                "tag": "input",
                "id": "ta",
                "type": "number",
                "placeholder": "输入环境温度",
                "value": "14.7",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "风速",
                "tag": "input",
                "id": "w",
                "type": "number",
                "placeholder": "输入风速",
                "value": "2.9",
                "required": true,
                "readonly": false,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "保温厚度",
                "tag": "input",
                "id": "delta",
                "type": "number",
                "placeholder": "输入保温厚度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "黑度",
                "tag": "input",
                "id": "epsilon",
                "type": "number",
                "placeholder": "",
                "value": "0.3",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "导热系数 λ=",
                "tag": "input",
                "id": "a",
                "type": "number",
                "placeholder": "",
                "value": "0.044",
                "required": true,
                "readonly": false,
                "other": "+",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "b",
                "type": "number",
                "placeholder": "",
                "value": "0.0002",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",                
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "c",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ") +",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "d",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "e",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ")^2 +",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "f",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "g",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ")^3",
                "pclass": "form-group inline",
                "class": "short"
            }
        ],
        "result": [
            {
                "name": "传热系数",
                "tag": "input",
                "id": "lambda",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "表面传热系数",
                "tag": "input",
                "id": "alpha",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m2*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "外表面温度",
                "tag": "input",
                "id": "ts",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位面积热损失",
                "tag": "input",
                "id": "Q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m2",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位长度热损失",
                "tag": "input",
                "id": "q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pipeCInsultion": {
        "text": "管道保冷计算",
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
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子外径",
                "tag": "input",
                "id": "d0",
                "type": "number",
                "placeholder": "输入管子外径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "物料温度",
                "tag": "input",
                "id": "t0",
                "type": "number",
                "placeholder": "输入物料温度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "环境温度",
                "tag": "input",
                "id": "ta",
                "type": "number",
                "placeholder": "输入环境温度",
                "value": "34.7",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "风速",
                "tag": "input",
                "id": "w",
                "type": "number",
                "placeholder": "输入风速",
                "value": "2.8",
                "required": true,
                "readonly": false,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "保冷厚度",
                "tag": "input",
                "id": "delta",
                "type": "number",
                "placeholder": "输入保冷厚度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "黑度",
                "tag": "input",
                "id": "epsilon",
                "type": "number",
                "placeholder": "",
                "value": "0.3",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "导热系数 λ=",
                "tag": "input",
                "id": "a",
                "type": "number",
                "placeholder": "",
                "value": "0.023",
                "required": true,
                "readonly": false,
                "other": "+",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "b",
                "type": "number",
                "placeholder": "",
                "value": "0.000122",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "c",
                "type": "number",
                "placeholder": "",
                "value": "25",
                "required": true,
                "readonly": false,
                "other": "+",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "d",
                "type": "number",
                "placeholder": "",
                "value": "3.15E-7",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "e",
                "type": "number",
                "placeholder": "",
                "value": "25",
                "required": true,
                "readonly": false,
                "other": "^2 +",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "f",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "g",
                "type": "number",
                "placeholder": "",
                "value": "25",
                "required": true,
                "readonly": false,
                "other": "^3",
                "pclass": "form-group inline",
                "class": "short"
            }
        ],
        "result": [
            {
                "name": "传热系数",
                "tag": "input",
                "id": "k",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "表面传热系数",
                "tag": "input",
                "id": "alpha",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m2*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "外表面温度",
                "tag": "input",
                "id": "ts",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位面积热损失",
                "tag": "input",
                "id": "Q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m2",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位长度热损失",
                "tag": "input",
                "id": "q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "waterPipe": {
        "text": "水和水蒸汽管道计算",
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
                "pclass": "form-group",
                "class": ""
            },{
                "name": "流量",
                "tag": "input",
                "id": "flowRate",
                "type": "number",
                "placeholder": "输入流量",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口温度",
                "tag": "input",
                "id": "a_t",
                "type": "number",
                "placeholder": "输入入口温度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口压力",
                "tag": "input",
                "id": "a_p",
                "type": "number",
                "placeholder": "输入入口压力",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "MPa",
                "unitset": "pressure_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子外径",
                "tag": "input",
                "id": "d0",
                "type": "number",
                "placeholder": "输入管子外径",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "管子壁厚",
                "tag": "input",
                "id": "delta",
                "type": "number",
                "placeholder": "输入管子壁厚",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "展开长度",
                "tag": "input",
                "id": "ll",
                "type": "number",
                "placeholder": "输入展开长度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "m",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "局部阻力当量长度",
                "tag": "input",
                "id": "lf",
                "type": "number",
                "placeholder": "输入局部阻力当量长度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "m",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "绝对粗糙度",
                "tag": "input",
                "id": "rough",
                "type": "number",
                "placeholder": "输入绝对粗糙度",
                "value": "0.2",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "保温厚度",
                "tag": "input",
                "id": "insuldelta",
                "type": "number",
                "placeholder": "输入保温厚度",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "mm",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "导热系数 λ=",
                "tag": "input",
                "id": "a",
                "type": "number",
                "placeholder": "",
                "value": "0.044",
                "required": true,
                "readonly": false,
                "other": "+",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "b",
                "type": "number",
                "placeholder": "",
                "value": "0.0002",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "c",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ") +",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "d",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "e",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ")^2 +",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "f",
                "type": "number",
                "placeholder": "",
                "value": "0",
                "required": true,
                "readonly": false,
                "other": "×(Tm-",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "",
                "tag": "input",
                "id": "g",
                "type": "number",
                "placeholder": "",
                "value": "70",
                "required": true,
                "readonly": false,
                "other": ")^3",
                "pclass": "form-group inline",
                "class": "short"
            },{
                "name": "黑度",
                "tag": "input",
                "id": "epsilon",
                "type": "number",
                "placeholder": "",
                "value": "0.3",
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "环境温度",
                "tag": "input",
                "id": "ta",
                "type": "number",
                "placeholder": "输入环境温度",
                "value": "12.7",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "风速",
                "tag": "input",
                "id": "w",
                "type": "number",
                "placeholder": "输入风速",
                "value": "2.9",
                "required": true,
                "readonly": false,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "压力降",
                "tag": "input",
                "id": "dp",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "MPa",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "温度降",
                "tag": "input",
                "id": "dt",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "K",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "传热系数",
                "tag": "input",
                "id": "lambda",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "表面传热系数",
                "tag": "input",
                "id": "alpha",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m2*K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "外表面温度",
                "tag": "input",
                "id": "ts",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位面积热损失",
                "tag": "input",
                "id": "Q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m2",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "单位长度热损失",
                "tag": "input",
                "id": "q",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "总热损失",
                "tag": "input",
                "id": "tq",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口压力",
                "tag": "input",
                "id": "b_p",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "MPa",
                "unitset": "pressure_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口温度",
                "tag": "input",
                "id": "b_t",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口流量（气相）",
                "tag": "input",
                "id": "a_f_gas",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口流量（液相）",
                "tag": "input",
                "id": "a_f_liquid",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口流量（气相）",
                "tag": "input",
                "id": "b_f_gas",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口流量（液相）",
                "tag": "input",
                "id": "b_f_liquid",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口流速",
                "tag": "input",
                "id": "a_ve",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口流速",
                "tag": "input",
                "id": "b_ve",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口比焓",
                "tag": "input",
                "id": "a_h",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kj/kg",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口比焓",
                "tag": "input",
                "id": "b_h",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kj/kg",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "入口干度",
                "tag": "input",
                "id": "a_x",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "出口干度",
                "tag": "input",
                "id": "b_x",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pumpPower": {
        "text": "泵轴功率计算",
        "form": [
            {
                "name": "编号",
                "tag": "input",
                "id": "pcode",
                "type": "text",
                "placeholder": "",
                "value": "P1",
                "required": false,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "名称",
                "tag": "input",
                "id": "pname",
                "type": "text",
                "placeholder": "输入名称",
                "value": "",
                "required": false,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "质量流量",
                "tag": "input",
                "id": "flowRate",
                "type": "number",
                "placeholder": "输入质量流量",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "kg/h",
                "unitset": "massFlowRate_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "扬程",
                "tag": "input",
                "id": "h",
                "type": "number",
                "placeholder": "输入扬程",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "m",
                "unitset": "length_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "效率",
                "tag": "input",
                "id": "eta",
                "type": "number",
                "placeholder": "输入效率",
                "value": "",
                "required": true,
                "readonly": false,
                "other": "%",
                "pclass": "form-group",
                "class": ""
            }
        ],
        "result": [
            {
                "name": "轴功率",
                "tag": "input",
                "id": "p",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kW",
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "waterProps": {
        "text": "水和水蒸汽物性计算",
        "form": [
            {
                "name": "介质",
                "tag": "input",
                "id": "fluid",
                "type": "text",
                "value": "水和水蒸汽",
                "required": true,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "参数1",
                "tag": "select",
                "id": "propArg1",
                "type": "",
                "value": "p",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "text": "压力",
                        "value": "p"
                    },{
                        "text": "温度",
                        "value": "T"
                    },{
                        "text": "比焓",
                        "value": "h"
                    }
                ],
                "listener": "onPropArg1Change()",
                "pclass": "form-group inline",
                "class": ""
            },{
                "name": "",
                "tag": "input",
                "id": "propValue1",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "MPaA",
                "unitset": "pressure_unit",
                "pclass": "form-group inline",
                "class": "short"
            },
            {
                "name": "参数2",
                "tag": "select",
                "id": "propArg2",
                "type": "",
                "value": "T",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "text": "温度",
                        "value": "T"
                    },{
                        "text": "密度",
                        "value": "rho"
                    },{
                        "text": "比焓",
                        "value": "h"
                    },{
                        "text": "干度",
                        "value": "x"
                    }
                ],
                "listener": "onPropArg2Change()",
                "pclass": "form-group inline br",
                "class": ""
            },{
                "name": "",
                "tag": "input",
                "id": "propValue2",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group inline",
                "class": "short"
            }
        ],
        "result": [
            {
                "name": "压力",
                "tag": "input",
                "id": "p",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "MPaA",
                "unitset": "pressure_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "温度",
                "tag": "input",
                "id": "T",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "比焓",
                "tag": "input",
                "id": "h",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kj/kg",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "比体积",
                "tag": "input",
                "id": "v",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "m3/kg",
                "unitset": "specificVolume_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "密度",
                "tag": "input",
                "id": "rho",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kg/m3",
                "unitset": "density_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "比熵",
                "tag": "input",
                "id": "s",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kj/(kg.K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "内能",
                "tag": "input",
                "id": "u",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kj/kg",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "定压比热容",
                "tag": "input",
                "id": "Cp",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kJ/(kg·K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "定容比热容",
                "tag": "input",
                "id": "Cv",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "kJ/(kg·K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "声速",
                "tag": "input",
                "id": "w",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "m/s",
                "unitset": "speed_unit",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "动力粘度",
                "tag": "input",
                "id": "my",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "Pa·s",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "导热系数",
                "tag": "input",
                "id": "tc",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "W/(m.K)",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "表面张力",
                "tag": "input",
                "id": "st",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "unit": "N/m",
                "unitset": "*",
                "pclass": "form-group",
                "class": ""
            },{
                "name": "干度",
                "tag": "input",
                "id": "x",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "气相体积分数",
                "tag": "input",
                "id": "vx",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "绝热指数",
                "tag": "input",
                "id": "k",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": false,
                "readonly": true,
                "pclass": "form-group",
                "class": ""
            }
        ]
    },
    "pureProps": {
        "text": "纯物质物性计算",
        "form": [
            {
                "name": "介质",
                "tag": "input",
                "id": "fluidName",
                "type": "text",
                "value": "",
                "datalist": [
                    "Acetone","Air","Ammonia","Argon","Benzene","CarbonDioxide","CarbonMonoxide","CarbonylSulfide","CycloHexane","CycloPropane","Cyclopentane","D4","D6","Deuterium","DiethylEther","DimethylCarbonate","DimethylEther","Ethane","D5","Dichloroethane","Ethanol","EthylBenzene","Ethylene","EthyleneOxide","Fluorine","HFE143m","HeavyWater","Helium","Hydrogen","HydrogenChloride","HydrogenSulfide","IsoButane","IsoButene","Isohexane","Isopentane","Krypton","MD2M","MD3M","MD4M","MDM","MM","Methane","Methanol","MethylLinoleate","MethylLinolenate","MethylOleate","MethylPalmitate","MethylStearate","Neon","Neopentane","Nitrogen","NitrousOxide","Novec649","OrthoDeuterium","OrthoHydrogen","Oxygen","ParaDeuterium","ParaHydrogen","Propylene","Propyne","R11","R113","R114","R115","R116","R12","R123","R1233zd(E)","R1234yf","R1234ze(E)","R1234ze(Z)","R124","R1243zf","R125","R13","R134a","R13I1","R14","R141b","R142b","R143a","R152A","R161","R21","R218","R22","R227EA","R23","R236EA","R236FA","R245ca","R245fa","R32","R365MFC","R40","R404A","R407C","R41","R410A","R507A","RC318","SES36","SulfurDioxide","SulfurHexafluoride","Toluene","Water","Xenon","cis-2-Butene","m-Xylene","n-Butane","n-Decane","n-Dodecane","n-Heptane","n-Hexane","n-Nonane","n-Octane","n-Pentane","n-Propane","n-Undecane","o-Xylene","p-Xylene","trans-2-Butene","1-Butene"
                ],
                "required": true,
                "readonly": false,
                "pclass": "form-group",
                "class": ""
            },{
                "name": "参数1",
                "tag": "select",
                "id": "propArg1",
                "type": "",
                "value": "P",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "text": "压力",
                        "value": "P"
                    },{
                        "text": "温度",
                        "value": "T"
                    },{
                        "text": "摩尔密度",
                        "value": "DMOLAR"
                    },{
                        "text": "质量密度",
                        "value": "D"
                    },{
                        "text": "摩尔比焓",
                        "value": "HMOLAR"
                    },{
                        "text": "质量比焓",
                        "value": "H"
                    },{
                        "text": "摩尔比熵",
                        "value": "SMOLAR"
                    },{
                        "text": "质量比熵",
                        "value": "S"
                    },{
                        "text": "摩尔内能",
                        "value": "UMOLAR"
                    },{
                        "text": "质量内能",
                        "value": "U"
                    },{
                        "text": "气体比率",
                        "value": "Q"
                    }
                ],
                "listener": "onPropArg1Change()",
                "pclass": "form-group inline",
                "class": ""
            },{
                "name": "",
                "tag": "input",
                "id": "propValue1",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "MPaA",
                "unitset": "pressure_unit",
                "pclass": "form-group inline",
                "class": "short"
            },
            {
                "name": "参数2",
                "tag": "select",
                "id": "propArg2",
                "type": "",
                "value": "T",
                "required": true,
                "readonly": false,
                "opts": [
                    {
                        "text": "温度",
                        "value": "T"
                    },{
                        "text": "摩尔密度",
                        "value": "DMOLAR"
                    },{
                        "text": "质量密度",
                        "value": "D"
                    },{
                        "text": "摩尔比焓",
                        "value": "HMOLAR"
                    },{
                        "text": "质量比焓",
                        "value": "H"
                    },{
                        "text": "摩尔比熵",
                        "value": "SMOLAR"
                    },{
                        "text": "质量比熵",
                        "value": "S"
                    },{
                        "text": "摩尔内能",
                        "value": "UMOLAR"
                    },{
                        "text": "质量内能",
                        "value": "U"
                    },{
                        "text": "气体比率",
                        "value": "Q"
                    }
                ],
                "listener": "onPropArg2Change()",
                "pclass": "form-group inline br",
                "class": ""
            },{
                "name": "",
                "tag": "input",
                "id": "propValue2",
                "type": "number",
                "placeholder": "",
                "value": "",
                "required": true,
                "readonly": false,
                "unit": "C",
                "unitset": "temperature_unit",
                "pclass": "form-group inline",
                "class": "short"
            }
        ],
        "result": [
            {"name": "压力", "tag": "input", "id": "P", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "MPaA", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            {"name": "温度", "tag": "input", "id": "T", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit":  "C", "unitset": "temperature_unit", "pclass": "form-group", "class": ""},
            {"name": "摩尔质量", "tag": "input", "id": "M", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            //{"name": "还原密度 (rho/rhoc)", "tag": "input", "id": "DELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "摩尔密度", "tag": "input", "id": "DMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "质量密度", "tag": "input", "id": "D", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔比焓", "tag": "input", "id": "HMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "质量比焓", "tag": "input", "id": "H", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔比熵", "tag": "input", "id": "SMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "质量比熵", "tag": "input", "id": "S", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""}, 
            {"name": "质量定容比热", "tag": "input", "id": "CVMASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔定容比热", "tag": "input", "id": "CVMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "质量比定压比热", "tag": "input", "id": "C", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔定压比热", "tag": "input", "id": "CPMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "理想气体质量定压比热", "tag": "input", "id": "CP0MASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "理想气体摩尔定压比热", "tag": "input", "id": "CP0MOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            {"name": "干度", "tag": "input", "id": "Q", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔气体常数", "tag": "input", "id": "GAS_CONSTANT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            {"name": "导热系数", "tag": "input", "id": "CONDUCTIVITY", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "W/(m.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "声速", "tag": "input", "id": "A", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "m/s", "unitset":  "speed_unit", "pclass":  "form-group", "class":  ""},
            //{"name": "表面张力", "tag": "input", "id": "I", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "N/m", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "粘度", "tag": "input", "id": "V", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa.s", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "压缩因子", "tag": "input", "id": "Z", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "等熵膨胀系数", "tag": "input", "id": "ISENTROPIC_EXPANSION_COEFFICIENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "等压膨胀系数", "tag": "input", "id": "ISOBARIC_EXPANSION_COEFFICIENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "1/K", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "等温压缩性", "tag": "input", "id": "ISOTHERMAL_COMPRESSIBILITY", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "1/Pa", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            //{"name": "倒数还原温度 (Tc/T)", "tag": "input", "id": "TAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "偏心因子", "tag": "input", "id": "ACENTRIC", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "偶极矩", "tag": "input", "id": "DIPOLE_MOMENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "C m", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "临界点压力", "tag": "input", "id": "PCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            
            {"name": "三相点的压力", "tag": "input", "id": "PTRIPLE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            {"name": "还原点压力", "tag": "input", "id": "P_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            {"name": "临界点质量密度", "tag": "input", "id": "RHOCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            //{"name": "还原点质量密度", "tag": "input", "id": "RHOMASS_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            {"name": "临界点摩尔密度", "tag": "input", "id": "RHOMOLAR_CRITICAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "还原点摩尔密度", "tag": "input", "id": "RHOMOLAR_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "残差摩尔熵", "tag": "input", "id": "SMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "临界点温度", "tag": "input", "id": "TCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            {"name": "最高温度限值", "tag": "input", "id": "TMAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            {"name": "最低温度限值", "tag": "input", "id": "TMIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            {"name": "三相点的温度", "tag": "input", "id": "TTRIPLE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            //{"name": "不可压缩溶液的冻结温度", "tag": "input", "id": "T_FREEZE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            {"name": "还原点温度", "tag": "input", "id": "T_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            {"name": "残余摩尔焓", "tag": "input", "id": "HMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            {"name": "质量比吉布斯能", "tag": "input", "id": "G", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔比吉布斯能", "tag": "input", "id": "GMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            {"name": "质量比亥姆霍兹能", "tag": "input", "id": "HELMHOLTZMASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "摩尔比亥姆霍兹能", "tag": "input", "id": "HELMHOLTZMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            {"name": "残余摩尔吉布斯能", "tag": "input", "id": "GMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            
            {"name": "理想亥姆霍兹能量", "tag": "input", "id": "ALPHA0", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "剩余亥姆霍兹能量", "tag": "input", "id": "ALPHAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "理想亥姆霍兹能与δ的导数", "tag": "input", "id": "DALPHA0_DDELTA_CONSTTAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "理想亥姆霍兹能与τ的导数", "tag": "input", "id": "DALPHA0_DTAU_CONSTDELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "剩余亥姆霍兹能量与δ的导数", "tag": "input", "id": "DALPHAR_DDELTA_CONSTTAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "剩余亥姆霍兹能量与τ的导数", "tag": "input", "id": "DALPHAR_DTAU_CONSTDELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "第二维里系数", "tag": "input", "id": "BVIRIAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "第三维里系数", "tag": "input", "id": "CVIRIA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            
            {"name": "第二维里系数对T的导数", "tag": "input", "id": "DBVIRIAL_DT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "第三维里系数对T的导数", "tag": "input", "id": "DCVIRIAL_DT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "不可压缩溶液分数（摩尔，质量，体积）的最大值", "tag": "input", "id": "FRACTION_MAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "不可压缩溶液分数（摩尔、质量、体积）的最小值", "tag": "input", "id": "FRACTION_MIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "最大压力限值", "tag": "input", "id": "PMAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            {"name": "最小压力限值", "tag": "input", "id": "PMIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            
            {"name": "气体动力学的基本导数", "tag": "input", "id": "FUNDAMENTAL_DERIVATIVE_OF_GAS_DYNAMICS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "百年全球变暖潜能值", "tag": "input", "id": "GWP100", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "20年全球变暖潜能值", "tag": "input", "id": "GWP20", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "500年全球变暖潜能值", "tag": "input", "id": "GWP500", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "可燃性危险", "tag": "input", "id": "FH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "健康危害", "tag": "input", "id": "HH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "臭氧消耗潜能值", "tag": "input", "id": "ODP", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "Phase index as a float", "tag": "input", "id": "PHASE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "物理危害", "tag": "input", "id": "PH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            //{"name": "Phase identification parameter", "tag": "input", "id": "PIP", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            {"name": "普朗特数", "tag": "input", "id": "PRANDTL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "还原密度 (rho/rhoc)", "tag": "input", "id": "DELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔密度", "tag": "input", "id": "DMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "质量密度", "tag": "input", "id": "D", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比焓", "tag": "input", "id": "HMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "质量比焓", "tag": "input", "id": "H", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "压力", "tag": "input", "id": "P", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "干度", "tag": "input", "id": "Q", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比熵", "tag": "input", "id": "SMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "质量比熵", "tag": "input", "id": "S", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "倒数还原温度 (Tc/T)", "tag": "input", "id": "TAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "温度", "tag": "input", "id": "T", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit":  "K", "unitset": "temperature_unit", "pclass": "form-group", "class": ""},
            // {"name": "摩尔比熵", "tag": "input", "id": "UMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比熵", "tag": "input", "id": "U", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "偏心因子", "tag": "input", "id": "ACENTRIC", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "理想亥姆霍兹能量", "tag": "input", "id": "ALPHA0", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "剩余亥姆霍兹能量", "tag": "input", "id": "ALPHAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "声速", "tag": "input", "id": "A", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "m/s", "unitset":  "speed_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "第二维里系数", "tag": "input", "id": "BVIRIAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "导热系数", "tag": "input", "id": "CONDUCTIVITY", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "W/(m.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "理想气体质量比恒压比热", "tag": "input", "id": "CP0MASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "理想气体摩尔比恒压比热", "tag": "input", "id": "CP0MOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比定压比热", "tag": "input", "id": "CPMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "第三维里系数", "tag": "input", "id": "CVIRIA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "质量比定容比热", "tag": "input", "id": "CVMASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比定容比热", "tag": "input", "id": "CVMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "质量比定压比热", "tag": "input", "id": "C", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(kg.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "理想亥姆霍兹能与δ的导数", "tag": "input", "id": "DALPHA0_DDELTA_CONSTTAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "理想亥姆霍兹能与τ的导数", "tag": "input", "id": "DALPHA0_DTAU_CONSTDELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "剩余亥姆霍兹能量与δ的导数", "tag": "input", "id": "DALPHAR_DDELTA_CONSTTAU", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "剩余亥姆霍兹能量与τ的导数", "tag": "input", "id": "DALPHAR_DTAU_CONSTDELTA", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "第二维里系数对T的导数", "tag": "input", "id": "DBVIRIAL_DT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "第三维里系数对T的导数", "tag": "input", "id": "DCVIRIAL_DT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "偶极矩", "tag": "input", "id": "DIPOLE_MOMENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "C m", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "可燃性危险", "tag": "input", "id": "FH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "不可压缩溶液分数（摩尔，质量，体积）的最大值", "tag": "input", "id": "FRACTION_MAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "不可压缩溶液分数（摩尔、质量、体积）的最小值", "tag": "input", "id": "FRACTION_MIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "气体动力学的基本导数", "tag": "input", "id": "FUNDAMENTAL_DERIVATIVE_OF_GAS_DYNAMICS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔气体常数", "tag": "input", "id": "GAS_CONSTANT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "残余摩尔吉布斯能", "tag": "input", "id": "GMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比吉布斯能", "tag": "input", "id": "GMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "百年全球变暖潜能值", "tag": "input", "id": "GWP100", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "20年全球变暖潜能值", "tag": "input", "id": "GWP20", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "500年全球变暖潜能值", "tag": "input", "id": "GWP500", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "质量比吉布斯能", "tag": "input", "id": "G", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "质量比亥姆霍兹能", "tag": "input", "id": "HELMHOLTZMASS", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/kg", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔比亥姆霍兹能", "tag": "input", "id": "HELMHOLTZMOLAR", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "健康危害", "tag": "input", "id": "HH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "残余摩尔焓", "tag": "input", "id": "HMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "等熵膨胀系数", "tag": "input", "id": "ISENTROPIC_EXPANSION_COEFFICIENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "等压膨胀系数", "tag": "input", "id": "ISOBARIC_EXPANSION_COEFFICIENT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "1/K", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "等温压缩性", "tag": "input", "id": "ISOTHERMAL_COMPRESSIBILITY", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "1/Pa", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "表面张力", "tag": "input", "id": "SURFACE_TENSION", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "N/m", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "摩尔质量", "tag": "input", "id": "M", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/mol", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "臭氧消耗潜能值", "tag": "input", "id": "ODP", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "临界点压力", "tag": "input", "id": "PCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "Phase index as a float", "tag": "input", "id": "PHASE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "物理危害", "tag": "input", "id": "PH", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "Phase identification parameter", "tag": "input", "id": "PIP", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "最大压力限值", "tag": "input", "id": "PMAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "最小压力限值", "tag": "input", "id": "PMIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "普朗特数", "tag": "input", "id": "PRANDTL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""},
            // {"name": "三相点的压力", "tag": "input", "id": "PTRIPLE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "还原点压力", "tag": "input", "id": "P_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa", "unitset":  "pressure_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "临界点质量密度", "tag": "input", "id": "RHOCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "还原点质量密度", "tag": "input", "id": "RHOMASS_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "kg/m3", "unitset":  "density_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "临界点摩尔密度", "tag": "input", "id": "RHOMOLAR_CRITICAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "还原点摩尔密度", "tag": "input", "id": "RHOMOLAR_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "mol/m3", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "残差摩尔熵", "tag": "input", "id": "SMOLAR_RESIDUAL", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "J/(mol.K)", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "临界点温度", "tag": "input", "id": "TCRIT", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "最高温度限值", "tag": "input", "id": "TMAX", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "最低温度限值", "tag": "input", "id": "TMIN", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "三相点的温度", "tag": "input", "id": "TTRIPLE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "不可压缩溶液的冻结温度", "tag": "input", "id": "T_FREEZE", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "还原点温度", "tag": "input", "id": "T_REDUCING", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "K", "unitset":  "temperature_unit", "pclass":  "form-group", "class":  ""},
            // {"name": "粘度", "tag": "input", "id": "V", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "unit": "Pa.s", "unitset":  "*", "pclass":  "form-group", "class":  ""},
            // {"name": "压缩因子", "tag": "input", "id": "Z", "type": "number", "placeholder":  "", "value":  "", "required":  false, "readonly":  true, "pclass":  "form-group", "class":  ""}
        ]
    }
}
export default db;