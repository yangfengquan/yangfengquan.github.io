import os
import json
from tkinter import messagebox
class PipeType:
    """管道类型类"""
    
    def __init__(self, name, roughness, description=""):
        self.name = name
        self.roughness = roughness  # 粗糙度，m
        self.description = description

class PipeFitting:
    """管道元件类"""
    
    def __init__(self, name, resistance_coef, description=""):
        self.name = name
        self.resistance_coef = resistance_coef  # 局部阻力系数
        self.description = description

class InsulationMaterial:
    """保温材料类"""
    
    def __init__(self, name, conductivity_eq1=None, conductivity_eq2=None, conductivity_eq3=None,
                 density=100, description=""):
        self.name = name
        self.conductivity_eq1 = conductivity_eq1
        self.conductivity_eq2 = conductivity_eq2
        self.conductivity_eq3 = conductivity_eq3
        self.density = density  # kg/m³
        self.description = description
    
    def calculate_conductivity(self, tm_k):
        """计算导热系数"""
        tm_c = tm_k -273.15
        try:
            if self.conductivity_eq1:
                eq = self.conductivity_eq1.split(":")
                if len(eq) > 1 and eval(eq[1], {"tm": tm_c}) or len(eq) == 1:
                    return eval(eq[0],{"tm": tm_c})
            if self.conductivity_eq2:
                eq = self.conductivity_eq2.split(":")
                if len(eq) > 1 and eval(eq[1], {"tm": tm_c}) or len(eq) == 1:
                    return eval(eq[0],{"tm": tm_c})
            if self.conductivity_eq3:
                eq = self.conductivity_eq3.split(":")
                if len(eq) > 1 and eval(eq[1], {"tm": tm_c}) or len(eq) == 1:
                    return eval(eq[0],{"tm": tm_c})

        except Exception as e:
            print(f"绝热材料导热系数计算错误:{e}")
            pass
        return None  # 默认值

class OuterProtection:
    """外保护层材料类"""
    
    def __init__(self, name, emissivity=0.3, description=""):
        self.name = name
        self.emissivity = emissivity  # 黑度
        self.description = description

class MaterialManager:
    """材料管理器"""
    
    def __init__(self):
        self.insulation_materials = {}
        self.protection_materials = {}
        self.pipe_fittings = {}
        self.pipe_types = {}
        if not self.load_materials_from_file():
            self.create_default_material_files()
            self.load_materials_from_file()
           
    
    def add_insulation_material(self, name, eq_1, eq_2, eq_3, density=100, description=""):
        """添加保温材料"""
        self.insulation_materials[name] = InsulationMaterial(
            name, eq_1, eq_2, eq_3,
            density=density, description=description
        )
    
    def add_protection_material(self, name, emissivity, description=""):
        """添加外保护层材料"""
        self.protection_materials[name] = OuterProtection(name, emissivity, description)
    
    def add_pipe_fitting(self, name, resistance_coef, description=""):
        """添加管道元件"""
        self.pipe_fittings[name] = PipeFitting(name, resistance_coef, description)
    
    def add_pipe_type(self, name,roughness, description=""):
        """添加管道类型"""
        self.pipe_types[name] = PipeType(name, roughness, description)
    
    def save_materials_to_file(self):
        """保存材料到文件"""
        try:
            # 保存保温材料
            insulation_data = {}
            for name, material in self.insulation_materials.items():
                insulation_data[name] = {
                    'conductivity_eq1': material.conductivity_eq1,
                    'conductivity_eq2': material.conductivity_eq2,
                    'conductivity_eq2': material.conductivity_eq3,
                    'density': material.density,
                    'description': material.description
                }
            
            with open('insulation_materials.json', 'w', encoding='utf-8') as f:
                json.dump(insulation_data, f, ensure_ascii=False, indent=2)
            
            # 保存外保护层材料
            protection_data = {}
            for name, material in self.protection_materials.items():
                protection_data[name] = {
                    'emissivity': material.emissivity,
                    'description': material.description
                }
            
            with open('protection_materials.json', 'w', encoding='utf-8') as f:
                json.dump(protection_data, f, ensure_ascii=False, indent=2)
            
            # 保存管道元件
            fittings_data = {}
            for name, fitting in self.pipe_fittings.items():
                fittings_data[name] = {
                    'resistance_coef': fitting.resistance_coef,
                    'description': fitting.description
                }
            
            with open('pipe_fittings.json', 'w', encoding='utf-8') as f:
                json.dump(fittings_data, f, ensure_ascii=False, indent=2)
                
            # 保存管道类型
            pipe_types_data = {}
            for name, pipe_type in self.pipe_types.items():
                pipe_types_data[name] = {
                    'roughness': pipe_type.roughness,
                    'description': pipe_type.description
                }
            
            with open('pipe_types.json', 'w', encoding='utf-8') as f:
                json.dump(pipe_types_data, f, ensure_ascii=False, indent=2)
                
            return True
        except Exception as e:
            messagebox.showerror("错误", f"保存材料文件失败: {e}")
            return False
    
    def load_materials_from_file(self):
        """从文件加载材料"""
        try:
            # 加载保温材料
            #if os.path.exists('insulation_materials.json'):
            with open('insulation_materials.json', 'r', encoding='utf-8') as f:
                insulation_data = json.load(f)
            
            # 确保数据是字典格式
            if isinstance(insulation_data, list):
                # 如果是列表，转换为字典格式
                insulation_data_dict = {}
                for item in insulation_data:
                    if isinstance(item, dict) and 'name' in item:
                        insulation_data_dict[item['name']] = item
                insulation_data = insulation_data_dict
            
            for name, data in insulation_data.items():
                # 兼容新旧格式
                if isinstance(data, dict):
                    self.add_insulation_material(
                        name, 
                        data.get('conductivity_eq1'),
                        data.get('conductivity_eq2'),
                        data.get('conductivity_eq3'),
                        data.get('density'),
                        data.get('description', '')
                    )
            
            # 加载外保护层材料
            #if os.path.exists('protection_materials.json'):
            with open('protection_materials.json', 'r', encoding='utf-8') as f:
                protection_data = json.load(f)
            
            # 确保数据是字典格式
            if isinstance(protection_data, list):
                protection_data_dict = {}
                for item in protection_data:
                    if isinstance(item, dict) and 'name' in item:
                        protection_data_dict[item['name']] = item
                protection_data = protection_data_dict
            
            for name, data in protection_data.items():
                if isinstance(data, dict):
                    self.add_protection_material(
                        name,
                        data.get('emissivity'),
                        data.get('description', '')
                    )
            
            # 加载管道元件
            #if os.path.exists('pipe_fittings.json'):
            with open('pipe_fittings.json', 'r', encoding='utf-8') as f:
                fittings_data = json.load(f)
            
            # 确保数据是字典格式
            if isinstance(fittings_data, list):
                fittings_data_dict = {}
                for item in fittings_data:
                    if isinstance(item, dict) and 'name' in item:
                        fittings_data_dict[item['name']] = item
                fittings_data = fittings_data_dict
            
            for name, data in fittings_data.items():
                if isinstance(data, dict):
                    self.add_pipe_fitting(
                        name,
                        data.get('resistance_coef'),
                        data.get('description', '')
                    )
            
            # 加载管道类型
            #if os.path.exists('pipe_types.json'):
            with open('pipe_types.json', 'r', encoding='utf-8') as f:
                pipe_types_data = json.load(f)
            
            # 确保数据是字典格式
            if isinstance(pipe_types_data, list):
                pipe_types_data_dict = {}
                for item in pipe_types_data:
                    if isinstance(item, dict) and 'name' in item:
                        pipe_types_data_dict[item['name']] = item
                pipe_types_data = pipe_types_data_dict
            
            for name, data in pipe_types_data.items():
                if isinstance(data, dict):
                    self.add_pipe_type(
                        name,
                        data.get('roughness', 0.0002),
                        data.get('description', '')
                    )
            
            print("材料文件加载成功")
            return True
            
        except Exception as e:
            print(f"加载材料文件失败: {e}")
            # 如果加载失败，创建默认文件
            #self.create_default_material_files()
            return False

    def create_default_material_files(self):
        """创建默认材料文件"""
        try:
            # 创建默认保温材料文件
            default_insulation = {
                "硅酸钙制品-I型-170": {
                    "conductivity_eq1": "0.0479+0.00010185*tm+9.65015e-11*tm**3:(tm<800)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "170",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸钙制品-II型-170": {
                    "conductivity_eq1": "0.0479+0.00010185*tm+9.65015e-11*tm**3:(tm<800)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "170",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸钙制品-I型-220": {
                    "conductivity_eq1": "0.0564+0.00007786*tm+7.8571e-8*tm**2:(tm<500)",
                    "conductivity_eq2": "0.0937+1.67397E-10*tm**3:(500<=tm<=800)",
                    "conductivity_eq3": "",
                    "density": "220",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸钙制品-II型-220": {
                    "conductivity_eq1": "0.0564+0.00007786*tm+7.8571e-8*tm**2:(tm<500)",
                    "conductivity_eq2": "0.0937+1.67397E-10*tm**3:(500<=tm<=800)",
                    "conductivity_eq3": "",
                    "density": "220",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "复合硅酸盐制品-涂料-180-200": {
                    "conductivity_eq1": "0.065+0.00017*(tm-70)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "180-200",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "复合硅酸盐制品-毡-60-80": {
                    "conductivity_eq1": "0.043+0.00015*(tm-70)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "60-80",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "复合硅酸盐制品-毡-81-130": {
                    "conductivity_eq1": "0.044+0.00015*(tm-70)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "81-130",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "复合硅酸盐制品-管壳-80-180": {
                    "conductivity_eq1": "0.048",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "80-180",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "岩棉制品-毡-60-100": {
                    "conductivity_eq1": "0.0337+0.000151*tm:(-20<=tm<=100)",
                    "conductivity_eq2": "0.0395+4.71e-5*tm+5.03e-7*tm**2:(100<tm<=600)",
                    "conductivity_eq3": "",
                    "density": "60-100",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "岩棉制品-缝毡-80-130": {
                    "conductivity_eq1": "0.0337+0.000128*tm:(-20<=tm<=100)",
                    "conductivity_eq2": "0.0407+2.52e-5*tm+3.34e-7*tm**2:(100<tm<=600)",
                    "conductivity_eq3": "",
                    "density": "80-130",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "岩棉制品-板-60-100": {
                    "conductivity_eq1": "0.0337+0.000151*tm:(-20<=tm<=100)",
                    "conductivity_eq2": "0.0395+4.71e-5*tm+5.03e-7*tm**2:(100<tm<=600)",
                    "conductivity_eq3": "",
                    "density": "60-100",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "岩棉制品-板-101-160": {
                    "conductivity_eq1": "0.0337+0.000128*tm:(-20<=tm<=100)",
                    "conductivity_eq2": "0.0407+2.52e-5*tm+3.34e-7*tm**2:(100<tm<=600)",
                    "conductivity_eq3": "",
                    "density": "101-160",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "岩棉制品-管壳-100-150": {
                    "conductivity_eq1": "0.0314+0.000174*tm:(-20<=tm<=100)",
                    "conductivity_eq2": "0.0384+7.13e-5*tm+3.51e-7*tm**2:(100<tm<=600)",
                    "conductivity_eq3": "",
                    "density": "100-150",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毯-24-40": {
                    "conductivity_eq1": "0.046+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "24-40",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毯-41-120": {
                    "conductivity_eq1": "0.041+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "41-120",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-板-24": {
                    "conductivity_eq1": "0.047+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "24",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-板-32": {
                    "conductivity_eq1": "0.044+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "32",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-板-40": {
                    "conductivity_eq1": "0.042+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "40",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-板-48": {
                    "conductivity_eq1": "0.041+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "48",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-板-64": {
                    "conductivity_eq1": "0.040+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "64",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毡-24": {
                    "conductivity_eq1": "0.046+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "24",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毡-32": {
                    "conductivity_eq1": "0.046+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "32",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毡-40": {
                    "conductivity_eq1": "0.046+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "40",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-毡-48": {
                    "conductivity_eq1": "0.041+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "48",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "玻璃棉制品-管壳-48": {
                    "conductivity_eq1": "0.041+0.000174*tm:(-20<=tm<=220)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": ">=48",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-1#毯-96": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "96",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-1#毯-128": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "128",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-2#毯-96": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "96",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-2#毯-128": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "128",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-1#毡-200": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "<=200",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-2#毡-200": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "<=200",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-板-220": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "<=220",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-管壳-220": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70):(tm<=400)",
                    "conductivity_eq2": "0.11+0.00036*(tm-400):(tm>400)",
                    "conductivity_eq3": "",
                    "density": "<=220",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸铝棉及其制品-树脂结合毡-128": {
                    "conductivity_eq1": "0.044+0.0002*(tm-70)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "128",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸镁纤维毯-100": {
                    "conductivity_eq1": "0.0397-2.741e-6*tm+4.526e-7*tm**2:(70<=tm<=500)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "100±10",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                },
                "硅酸镁纤维毯-130": {
                    "conductivity_eq1": "0.0397-2.741e-6*tm+4.526e-7*tm**2:(70<=tm<=500)",
                    "conductivity_eq2": "",
                    "conductivity_eq3": "",
                    "density": "130±10",
                    "description": "引自GB/50264-2013 附录A 表A.0.1"
                }
            }
            with open('insulation_materials.json', 'w', encoding='utf-8') as f:
                json.dump(default_insulation, f, ensure_ascii=False, indent=2)
            
            # 创建默认外保护层材料文件
            default_protection = {
                "铝合金薄板": {
                    "emissivity": 0.3,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "不锈钢薄板": {
                    "emissivity": 0.4,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "有光泽的镀钵薄钢板": {
                    "emissivity": 0.27,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "已氧化的镀钵薄钢板": {
                    "emissivity": 0.32,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "纤维织物": {
                    "emissivity": 0.80,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "水泥砂浆": {
                    "emissivity": 0.69,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "铝粉漆": {
                    "emissivity": 0.41,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "黑漆(有光泽)": {
                    "emissivity": 0.88,
                    "description": "引自GB/50264-2013 表5.8.9"
                },
                "黑漆(无光泽)": {
                    "emissivity": 0.96,
                    "description": "引自GB/50264-2013 表5.8.9"
                }
                ,
                "油漆": {
                    "emissivity": 0.90,
                    "description": "引自GB/50264-2013 表5.8.9"
                }
            }
            
            with open('protection_materials.json', 'w', encoding='utf-8') as f:
                json.dump(default_protection, f, ensure_ascii=False, indent=2)
            
            # 创建默认管道元件文件
            default_fittings = {
                "45°标准弯头": {
                    "resistance_coef": 0.35,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "90°标准弯头": {
                    "resistance_coef": 0.75,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "90°斜接弯头": {
                    "resistance_coef": 1.3,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "180°标准弯头": {
                    "resistance_coef": 1.5,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "等径三通(流出)": {
                    "resistance_coef": 1.2,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "等径三通(流入)": {
                    "resistance_coef": 1.8,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "截止阀(全开)": {
                    "resistance_coef": 6.0,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "角阀(全开)": {
                    "resistance_coef": 3.0,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "闸阀(全开)": {
                    "resistance_coef": 0.17,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "旋塞阀(全开)": {
                    "resistance_coef": 0.05,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "蝶阀(全开)": {
                    "resistance_coef": 0.24,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "旋启式止回阀": {
                    "resistance_coef": 2.0,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "升降式止回阀": {
                    "resistance_coef": 10.0,
                    "description": "引自SH/3035-2018 表6.2.5"
                },
                "底阀": {
                    "resistance_coef": 15.0,
                    "description": "引自SH/3035-2018 表6.2.5"
                }
            }
            
            with open('pipe_fittings.json', 'w', encoding='utf-8') as f:
                json.dump(default_fittings, f, ensure_ascii=False, indent=2)
            
            # 创建默认管道类型文件
            default_pipe_types = {
                "无缝黄铜、铜及铅管": {
                    "roughness": 0.00001,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.000005-0.00001"
                },
                "操作中基本无腐蚀的无缝钢管": {
                    "roughness": 0.0001,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.00005-0.0001"
                },
                "操作中有轻度腐蚀的无缝钢管": {
                    "roughness": 0.0002,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.0001-0.0002"
                },
                "操作中有显著腐蚀的无缝钢管": {
                    "roughness": 0.0005,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.0002-0.0005"
                },
                "铜板卷管": {
                    "roughness": 0.00033,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.00033"
                },
                "铸铁管": {
                    "roughness": 0.00085,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.0005-0.00085"
                },
                "干净的玻璃管": {
                    "roughness": 0.00001,
                    "description": "引自SH/3035-2018 表6.2.4 推荐值0.0000015-0.00001"
                }
            }
            
            with open('pipe_types.json', 'w', encoding='utf-8') as f:
                json.dump(default_pipe_types, f, ensure_ascii=False, indent=2)
            
            print("默认材料文件创建成功")
            return True
            
        except Exception as e:
            print(f"创建默认材料文件失败: {e}")
            return False