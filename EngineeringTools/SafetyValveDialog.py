import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
import math
import CoolProp.CoolProp as CP
from typing import Dict, Any, Optional

class FluidPropertiesDialog(simpledialog.Dialog):
    """流体物性参数输入对话框"""
    
    def __init__(self, parent, title="流体物性参数"):
        self.fluid_name = tk.StringVar(value="Water")
        self.pressure = tk.DoubleVar(value=1.0)
        self.temperature = tk.DoubleVar(value=100)
        self.quality = tk.DoubleVar(value=1.0)
        self.input_method = tk.StringVar(value="PQ")  # PT or PQ
        
        super().__init__(parent, title)
    
    def body(self, master):
        ttk.Label(master, text="流体名称:").grid(row=0, column=0, sticky="w", padx=5, pady=5)
        fluid_combo = ttk.Combobox(master, textvariable=self.fluid_name, width=20)
        fluid_combo['values'] = sorted(['Water', 'Propane', 'Air', 'Nitrogen', 'Oxygen', 
                                       'CarbonDioxide', 'Ammonia', 'Methane', 'Ethane', 
                                       'IsoButane', 'n-Butane', 'R134a'])
        fluid_combo.grid(row=0, column=1, padx=5, pady=5)
        
        ttk.Label(master, text="压力 (MPa):").grid(row=1, column=0, sticky="w", padx=5, pady=5)
        ttk.Entry(master, textvariable=self.pressure, width=15).grid(row=1, column=1, padx=5, pady=5)
        
        # 输入方法选择
        ttk.Label(master, text="输入方法:").grid(row=2, column=0, sticky="w", padx=5, pady=5)
        ttk.Radiobutton(master, text="压力-温度(PT)", variable=self.input_method, 
                       value="PT", command=self.toggle_input_method).grid(row=2, column=1, sticky="w")
        ttk.Radiobutton(master, text="压力-干度(PQ)", variable=self.input_method, 
                       value="PQ", command=self.toggle_input_method).grid(row=2, column=2, sticky="w")
        
        # 温度输入
        self.temp_label = ttk.Label(master, text="温度 (℃):")
        self.temp_label.grid(row=3, column=0, sticky="w", padx=5, pady=5)
        self.temp_entry = ttk.Entry(master, textvariable=self.temperature, width=15)
        self.temp_entry.grid(row=3, column=1, padx=5, pady=5)
        
        # 干度输入
        self.qual_label = ttk.Label(master, text="干度 [0-1]:")
        self.qual_entry = ttk.Entry(master, textvariable=self.quality, width=15)
        
        self.toggle_input_method()
        return fluid_combo  # 初始焦点
    
    def toggle_input_method(self):
        if self.input_method.get() == "PT":
            self.temp_label.grid()
            self.temp_entry.grid()
            self.qual_label.grid_remove()
            self.qual_entry.grid_remove()
        else:
            self.temp_label.grid_remove()
            self.temp_entry.grid_remove()
            self.qual_label.grid()
            self.qual_entry.grid(row=3, column=1, padx=5, pady=5)
    
    def apply(self):
        self.result = {
            'fluid': self.fluid_name.get(),
            'pressure': self.pressure.get(),
            'input_method': self.input_method.get(),
            'temperature': self.temperature.get() + 273.15 if self.input_method.get() == "PT" else None,
            'quality': self.quality.get() if self.input_method.get() == "PQ" else None
        }

class FireExposureDialog(simpledialog.Dialog):
    """火灾工况参数输入对话框 - 区分有保温和无保温"""
    
    def __init__(self, parent, title="火灾工况参数"):
        self.insulation_type = tk.StringVar(value="no_insulation")
        super().__init__(parent, title)
    
    def body(self, master):
        row = 0
        
        ttk.Label(master, text="保温类型:", font=('Arial', 10, 'bold')).grid(
            row=row, column=0, sticky="w", padx=5, pady=10)
        
        ttk.Radiobutton(master, text="无保温", variable=self.insulation_type,
                       value="no_insulation", command=self.toggle_insulation_type).grid(
                       row=row, column=1, sticky="w")
        ttk.Radiobutton(master, text="有保温", variable=self.insulation_type,
                       value="with_insulation", command=self.toggle_insulation_type).grid(
                       row=row, column=2, sticky="w")
        row += 1
        
        # 无保温参数
        self.no_ins_frame = ttk.Frame(master)
        self.no_ins_frame.grid(row=row, column=0, columnspan=3, sticky="w", padx=20, pady=5)
        
        ttk.Label(self.no_ins_frame, text="无保温容器参数", font=('Arial', 9, 'bold')).grid(
            row=0, column=0, columnspan=2, sticky="w", pady=5)
        
        ttk.Label(self.no_ins_frame, text="容器受热面积 A_r (m²):").grid(
            row=1, column=0, sticky="w", padx=5, pady=2)
        self.ar_var = tk.DoubleVar(value=100.0)
        ttk.Entry(self.no_ins_frame, textvariable=self.ar_var, width=12).grid(
            row=1, column=1, padx=5, pady=2)
        
        ttk.Label(self.no_ins_frame, text="系数 F:").grid(
            row=2, column=0, sticky="w", padx=5, pady=2)
        self.f_var = tk.DoubleVar(value=1.0)
        ttk.Entry(self.no_ins_frame, textvariable=self.f_var, width=12).grid(
            row=2, column=1, padx=5, pady=2)
        ttk.Label(self.no_ins_frame, text="(容器置于地面以下用沙土覆盖:0.3, 容器置于地面上:1.0, 容器置于大于10L/(m²*min)喷淋装置下:0.6)").grid(
            row=2, column=2, sticky="w", padx=5, pady=2)
        
        # 有保温参数
        self.with_ins_frame = ttk.Frame(master)
        self.with_ins_frame.grid(row=row, column=0, columnspan=3, sticky="w", padx=20, pady=5)
        
        ttk.Label(self.with_ins_frame, text="有保温容器参数", font=('Arial', 9, 'bold')).grid(
            row=0, column=0, columnspan=2, sticky="w", pady=5)
        
        ttk.Label(self.with_ins_frame, text="容器受热面积 A_r (m²):").grid(
            row=1, column=0, sticky="w", padx=5, pady=2)
        self.aw_var = tk.DoubleVar(value=120.0)
        ttk.Entry(self.with_ins_frame, textvariable=self.aw_var, width=12).grid(
            row=1, column=1, padx=5, pady=2)
        
        ttk.Label(self.with_ins_frame, text="保温层厚度 δ (m):").grid(
            row=2, column=0, sticky="w", padx=5, pady=2)
        self.ins_thick_var = tk.DoubleVar(value=0.1)
        ttk.Entry(self.with_ins_frame, textvariable=self.ins_thick_var, width=12).grid(
            row=2, column=1, padx=5, pady=2)
        
        ttk.Label(self.with_ins_frame, text="保温材料导热系数 λ (W/m·K):").grid(
            row=3, column=0, sticky="w", padx=5, pady=2)
        self.lambda_ins_var = tk.DoubleVar(value=0.1)
        ttk.Entry(self.with_ins_frame, textvariable=self.lambda_ins_var, width=12).grid(
            row=3, column=1, padx=5, pady=2)
        
        self.toggle_insulation_type()
        return self.no_ins_frame
    
    def toggle_insulation_type(self):
        if self.insulation_type.get() == "no_insulation":
            self.no_ins_frame.grid()
            self.with_ins_frame.grid_remove()
        else:
            self.no_ins_frame.grid_remove()
            self.with_ins_frame.grid()
    
    def apply(self):
        if self.insulation_type.get() == "no_insulation":
            self.result = {
                'insulation_type': 'no_insulation',
                'A_r': self.ar_var.get(),
                'f': self.f_var.get()
            }
        else:
            self.result = {
                'insulation_type': 'with_insulation',
                'A_w': self.aw_var.get(),
                'insulation_thickness': self.ins_thick_var.get(),
                'lambda_ins': self.lambda_ins_var.get()
            }

class ScenarioParametersDialog(simpledialog.Dialog):
    """工况参数输入对话框"""
    
    def __init__(self, parent, scenario_type, title="工况参数"):
        self.scenario_type = scenario_type
        self.parameters = {}
        super().__init__(parent, title)
    
    def body(self, master):
        row = 0
        
        if self.scenario_type == 'fire_exposure':
            # 使用专门的火灾工况对话框
            return None
            
        elif self.scenario_type == 'tube_rupture':
            ttk.Label(master, text="换热管破裂工况参数", font=('Arial', 10, 'bold')).grid(
                row=row, column=0, columnspan=2, pady=10)
            row += 1
            
            ttk.Label(master, text="换热管流通面积 (mm²):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
            self.atube_var = tk.DoubleVar(value=500.0)
            ttk.Entry(master, textvariable=self.atube_var, width=15).grid(row=row, column=1, padx=5, pady=5)
            row += 1
            
            ttk.Label(master, text="管壳程压差 (MPa):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
            self.delta_p_var = tk.DoubleVar(value=3.0)
            ttk.Entry(master, textvariable=self.delta_p_var, width=15).grid(row=row, column=1, padx=5, pady=5)
            row += 1
            
        elif self.scenario_type == 'thermal_vaporization':
            ttk.Label(master, text="换热设备产生蒸汽", font=('Arial', 10, 'bold')).grid(
                row=row, column=0, columnspan=2, pady=10)
            row += 1
            
            ttk.Label(master, text="传入热量 (kJ/h):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
            self.heat_input_var = tk.DoubleVar(value=100000.0)
            ttk.Entry(master, textvariable=self.heat_input_var, width=15).grid(row=row, column=1, padx=5, pady=5)
            row += 1
            
        elif self.scenario_type == 'gas_inflow':
            ttk.Label(master, text="气体流入工况参数", font=('Arial', 10, 'bold')).grid(
                row=row, column=0, columnspan=2, pady=10)
            row += 1
            
            ttk.Label(master, text="最大进气流量 (kg/h):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
            self.inflow_var = tk.DoubleVar(value=5000.0)
            ttk.Entry(master, textvariable=self.inflow_var, width=15).grid(row=row, column=1, padx=5, pady=5)
            row += 1
    
    def apply(self):
        if self.scenario_type == 'tube_rupture':
            self.result = {
                'A_tube': self.atube_var.get(),
                'delta_P': self.delta_p_var.get(),
                'Cd': 0.6
            }
        elif self.scenario_type == 'thermal_vaporization':
            self.result = {
                'heat_input': self.heat_input_var.get()
            }
        elif self.scenario_type == 'gas_inflow':
            self.result = {
                'max_inflow_rate': self.inflow_var.get()
            }

class SafetyValveCalculator:
    """安全阀计算核心类 - 严格遵循GB150标准"""
    
    #def __init__(self):
        
    
    def get_fluid_properties(self, fluid: str, P: float, T: float = None, Q: float = None) -> Dict[str, float]:
        """获取流体物性"""
        try:
            P_pa = P * 1e6
            
            if T is not None:
                state = CP.AbstractState('HEOS', fluid)
                state.update(CP.PT_INPUTS, P_pa, T)
            elif Q is not None:
                state = CP.AbstractState('HEOS', fluid)
                state.update(CP.PQ_INPUTS, P_pa, Q)
            else:
                raise ValueError("必须提供温度或干度")
            
            properties = {
                'T': state.T(),
                'P': state.p() / 1e6,
                'rho': state.rhomass(),
                'h': state.hmass(),
                's': state.smass(),
                'cp': state.cpmass(),
                'cv': state.cvmass(),
                'k': state.cpmass() / state.cvmass(),
                'M': state.molar_mass(),
                'Z': state.compressibility_factor(),
            }
            
            # 获取汽化潜热
            try:      
                state_liq = CP.AbstractState('HEOS', fluid)
                state_vap = CP.AbstractState('HEOS', fluid)
                state_liq.update(CP.PQ_INPUTS, P_pa, 0.0)
                state_vap.update(CP.PQ_INPUTS, P_pa, 1.0)
                properties['H_vap'] = (state_vap.hmass() - state_liq.hmass()) / 1000
                properties['T_sat'] = state_liq.T() - 273.
            except Exception as e:
                # 对于非凝结性流体，设置默认值
                properties['H_vap'] = properties.get('h', 0) / 1000
                properties['T_sat'] = properties['T'] - 273.
                raise ValueError(f"汽化潜热计算失败: {str(e)}")
            
            return properties
            
        except Exception as e:
            raise ValueError(f"物性计算失败: {str(e)}")
    
    def _get_gas_characteristic_factor_X(self, k: float) -> float:
        """计算气体特性系数X - GB150公式"""
        part_k = (2 / (k + 1)) ** ((k + 1) / (k - 1))
        C_val = math.sqrt(k * part_k)
        return 520 * C_val
    
    def _check_critical_flow(self, P1: float, Pb: float, k: float) -> tuple:
        """检查流动状态 - GB150 B.5.2"""
        # P1泄放压力，Pb背压
        critical_pressure_ratio = (2 / (k + 1)) ** (k / (k - 1))
        is_critical = (Pb / P1) <= critical_pressure_ratio
        return is_critical, critical_pressure_ratio
    
    def calculate_required_capacity(self, scenario: str, fluid_props: Dict, **kwargs) -> float:
        """计算安全泄放量 - 严格遵循GB150公式"""
        if scenario == 'fire_exposure':
            t_sat = fluid_props.get('T_sat', 50)  # °C
            H_vap = fluid_props.get('H_vap', 2000)  # kJ/kg
            
            if kwargs.get('insulation_type') == 'with_insulation':
                # 有保温容器 - GB150公式
                lambda_ins = kwargs.get('lambda_ins', 0.1)  # W/(m·K)
                A_r = kwargs.get('A_r', 100)  # m²
                delta = kwargs.get('insulation_thickness', 0.1)  # m
                
                Ws = (2.61 * (650 - t_sat) * lambda_ins * math.pow(A_r, 0.82)) / (H_vap * delta)
            else:
                # 无保温容器 - GB150公式
                A_r = kwargs.get('A_r', 100)  # m²
                f = kwargs.get('f', 1.0)  # 系数
                
                Ws = 2.55e5 * f * math.pow(A_r, 0.82) / H_vap
                
            return max(Ws, 0.0)  # 确保非负
                
        elif scenario == 'tube_rupture':
            Cd = kwargs.get('Cd', 0.6)
            A_tube = kwargs.get('A_tube', 500)  # mm²
            rho = fluid_props.get('rho', 1000)  # kg/m³
            delta_P = kwargs.get('delta_P', 3.0)  # MPa
            
            # GB150公式：Ws = 0.556 × Cd × A_t × (2 × ρ × ΔP × 10^6)^0.5
            Ws = 0.556 * Cd * A_tube * math.sqrt(2 * rho * delta_P * 1e6)
            return Ws
            
        elif scenario == 'thermal_vaporization':
            q = kwargs.get('heat_input', 100000)  # kJ/h
            H_vap = fluid_props.get('H_vap', 2000)  # kJ/kg
            
            # GB150公式：Ws = q / ΔH
            Ws = q / H_vap
            return Ws
            
        elif scenario == 'gas_inflow':
            return kwargs.get('max_inflow_rate', 5000)
        
        return 0.0
    
    def calculate_flow_area(self, fluid_type: str, W_s: float, P1: float, 
                          fluid_props: Dict, Pb: float = 0.101, K: float = 0.9) -> Dict:
        """计算泄放面积 - 严格遵循GB150公式"""
        result = {
            'flow_area': 0.0,
            'unit': 'mm²',
            'flow_type': '',
            'is_critical': False,
            'critical_ratio': 0.0,
            'warning': ''
        }
        
        try:
            if fluid_type.lower() == '气体':
                # 气体 - GB150 B.5.2公式
                k = fluid_props['k']
                M = fluid_props['M']
                T1 = fluid_props['T']
                Z = fluid_props.get('Z', 1.0)
                
                is_critical, critical_ratio = self._check_critical_flow(P1, Pb, k)
                X = self._get_gas_characteristic_factor_X(k)
                
                if is_critical:
                    # 临界流动
                    flow_type = "气体（临界流动）"
                    A_mm2 = 13.16 * W_s * math.sqrt(Z * T1 / M) / (K * X * P1)
                else:
                    # 亚临界流动
                    flow_type = "气体（亚临界流动）"
                    A_mm2 = 1.79e-2 * W_s * math.sqrt(Z * T1 / M) / (K * P1 * math.sqrt(k / (k - 1) * (math.pow(Pb / P1, 2 / k) - math.pow(Pb / P1, (k + 1) / k))))
                
                result.update({
                    'flow_area': A_mm2,
                    'flow_type': flow_type,
                    'is_critical': is_critical,
                    'critical_ratio': critical_ratio,
                    'X_factor': X
                })
                
            elif fluid_type.lower() == '饱和蒸汽':
                if P1 <= 10:
                    A_mm2 = 0.19 * W_s / (K * P1)
                elif P1 <= 22:
                    A_mm2 = 0.19 * W_s /  (K * P1) * ((33.2 * P1 - 1061) / (27.6 * P1 - 1000))
                result.update({
                    'flow_area': A_mm2,
                    'flow_type': f"饱和蒸汽",
                })
                
            elif fluid_type.lower() == '液体':
                # 液体 - GB150 B.5.4公式（仅适用于非粘滞性流体）
                rho = fluid_props['rho']  # kg/m³
                Kv = 1.0  # 粘度校正系数，非粘滞性流体取1.0
                delta_P = P1 - Pb  # MPa
                
                if delta_P <= 0:
                    raise ValueError("液体计算需要正压差")
                
                A_mm2 = 0.196 * W_s / (K * Kv * math.sqrt(rho * delta_P))
                
                result.update({
                    'flow_area': A_mm2,
                    'flow_type': f"液体（非粘滞性）",
                    'delta_P': delta_P,
                    'warning': '注意：此公式仅适用于非粘滞性流体，粘滞性流体需进行粘度修正'
                })
                
        except Exception as e:
            result['warning'] = str(e)
            
        return result

import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
import math
import CoolProp.CoolProp as CP
from typing import Dict, Any, Optional

class SafetyValveDialog(tk.Toplevel):
    """安全阀计算对话框 - 使用Toplevel"""
    
    def __init__(self, parent):
        super().__init__(parent)
        self.calculator = SafetyValveCalculator()
        self.fluid_props = None
        self.scenario_params = None
        self.current_scenario = None
        
        self.title("安全阀泄放面积计算")
        self.geometry("800x600")
        self.transient(parent)  # 设置为父窗口的临时窗口
        self.grab_set()  # 模态对话框
        
        self.create_widgets()
        self.center_window()
    
    def center_window(self):
        """居中显示窗口"""
        self.update_idletasks()
        width = self.winfo_width()
        height = self.winfo_height()
        x = (self.winfo_screenwidth() // 2) - (width // 2)
        y = (self.winfo_screenheight() // 2) - (height // 2)
        self.geometry(f'{width}x{height}+{x}+{y}')
    
    def create_widgets(self):
        # 主框架
        main_frame = ttk.Frame(self, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 标题
        title_label = ttk.Label(main_frame, text="安全阀泄放面积计算", 
                               font=('Arial', 16, 'bold'))
        title_label.grid(row=0, column=0, columnspan=2, pady=20)
        
        # 输入区域
        input_frame = ttk.LabelFrame(main_frame, text="计算参数", padding="10")
        input_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=10)
        
        # 流体物性按钮
        ttk.Button(input_frame, text="1. 设置流体物性", 
                  command=self.set_fluid_properties).grid(row=0, column=0, padx=5, pady=5)
        
        self.fluid_status = ttk.Label(input_frame, text="未设置", foreground="red")
        self.fluid_status.grid(row=0, column=1, padx=5, pady=5)
        
        # 工况选择
        ttk.Label(input_frame, text="2. 选择超压工况:").grid(row=1, column=0, sticky="w", pady=5)
        
        self.scenario_var = tk.StringVar()
        scenarios = [
            ('火灾工况', 'fire_exposure'),
            ('换热管破裂', 'tube_rupture'), 
            ('换热设备产生蒸汽', 'thermal_vaporization'),
            ('气体流入', 'gas_inflow')
        ]
        
        for i, (text, value) in enumerate(scenarios):
            ttk.Radiobutton(input_frame, text=text, variable=self.scenario_var,
                           value=value, command=self.on_scenario_change).grid(
                           row=2, column=i, padx=5, pady=5)
        
        self.scenario_status = ttk.Label(input_frame, text="未选择", foreground="red")
        self.scenario_status.grid(row=3, column=0, columnspan=4, pady=5)
        
        # 泄放压力
        ttk.Label(input_frame, text="3. 泄放压力 (MPa):").grid(row=4, column=0, sticky="w", pady=5)
        self.pressure_var = tk.DoubleVar(value=1.0)
        ttk.Entry(input_frame, textvariable=self.pressure_var, width=10).grid(row=4, column=1, padx=5, pady=5)
        
        # 背压
        ttk.Label(input_frame, text="背压 (MPa):").grid(row=4, column=2, sticky="w", pady=5)
        self.backpressure_var = tk.DoubleVar(value=0.101)
        ttk.Entry(input_frame, textvariable=self.backpressure_var, width=10).grid(row=4, column=3, padx=5, pady=5)
        
        # 介质类型
        ttk.Label(input_frame, text="4. 介质类型:").grid(row=5, column=0, sticky="w", pady=5)
        self.fluid_type_var = tk.StringVar(value="气体")
        ttk.Combobox(input_frame, textvariable=self.fluid_type_var, 
                    values=["气体", "饱和蒸汽", "液体"], width=10).grid(row=5, column=1, padx=5, pady=5)
        
        # 泄放装置的泄放系数
        ttk.Label(input_frame, text="5. 泄放装置的泄放系数:").grid(row=6, column=0, sticky="w", pady=5)
        self.K_var = tk.DoubleVar(value=0.9)
        ttk.Entry(input_frame, textvariable=self.K_var, width=10).grid(row=6, column=1, padx=5, pady=5)
        
        # 按钮框架
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=2, column=0, columnspan=2, pady=20)
        
        # 计算按钮
        ttk.Button(button_frame, text="开始计算", command=self.calculate).grid(row=0, column=0, padx=10)
        
        # 关闭按钮
        ttk.Button(button_frame, text="关闭", command=self.destroy).grid(row=0, column=1, padx=10)
        
        # 结果显示区域
        result_frame = ttk.LabelFrame(main_frame, text="计算结果", padding="10")
        result_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=10)
        
        self.result_text = tk.Text(result_frame, height=15, width=80)
        self.result_text.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 滚动条
        scrollbar = ttk.Scrollbar(result_frame, orient="vertical", command=self.result_text.yview)
        scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        self.result_text.configure(yscrollcommand=scrollbar.set)
        
        # 配置权重
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(3, weight=1)
        result_frame.columnconfigure(0, weight=1)
        result_frame.rowconfigure(0, weight=1)
    
    def set_fluid_properties(self):
        """设置流体物性"""
        dialog = FluidPropertiesDialog(self)
        if dialog.result:
            try:
                fluid_info = dialog.result
                if fluid_info['input_method'] == 'PT':
                    self.fluid_props = self.calculator.get_fluid_properties(
                        fluid_info['fluid'], fluid_info['pressure'], 
                        T=fluid_info['temperature'])
                else:
                    self.fluid_props = self.calculator.get_fluid_properties(
                        fluid_info['fluid'], fluid_info['pressure'], 
                        Q=fluid_info['quality'])
                
                self.fluid_status.config(text=f"已设置: {fluid_info['fluid']}", foreground="green")
                
            except Exception as e:
                messagebox.showerror("错误", f"物性计算失败: {str(e)}")
    
    def on_scenario_change(self):
        """工况改变事件"""
        scenario = self.scenario_var.get()
        if scenario:
            if scenario == 'fire_exposure':
                dialog = FireExposureDialog(self)
            else:
                dialog = ScenarioParametersDialog(self, scenario)
            
            if dialog.result:
                self.scenario_params = dialog.result
                self.current_scenario = scenario
                scenario_names = {
                    'fire_exposure': '火灾工况',
                    'tube_rupture': '换热管破裂',
                    'thermal_vaporization': '换热设备产生蒸汽',
                    'gas_inflow': '气体流入'
                }
                self.scenario_status.config(
                    text=f"已选择: {scenario_names[scenario]}", 
                    foreground="green"
                )
    
    def calculate(self):
        """执行计算"""
        if not self.fluid_props:
            messagebox.showerror("错误", "请先设置流体物性")
            return
        
        if not self.current_scenario:
            messagebox.showerror("错误", "请先选择超压工况")
            return
        
        try:
            # 计算安全泄放量
            W_s = self.calculator.calculate_required_capacity(
                self.current_scenario, self.fluid_props, **self.scenario_params)
            
            # 计算泄放面积
            result = self.calculator.calculate_flow_area(
                self.fluid_type_var.get(), W_s, self.pressure_var.get(),
                self.fluid_props, self.backpressure_var.get(), self.K_var.get())
            
            # 显示结果
            self.display_results(W_s, result)
            
        except Exception as e:
            messagebox.showerror("计算错误", str(e))
    
    def display_results(self, W_s: float, result: Dict):
        """显示计算结果"""
        self.result_text.delete(1.0, tk.END)
        
        output = []
        output.append("=" * 60)
        output.append("               安全阀计算报告")
        output.append("=" * 60)
        output.append("")
        
        output.append("【输入参数】")
        output.append(f"泄放装置的泄放系数: {self.K_var.get()}")
        output.append(f"泄放压力: {self.pressure_var.get()} MPa")
        output.append(f"背压: {self.backpressure_var.get()} MPa")
        output.append(f"介质类型: {self.fluid_type_var.get()}")
        output.append("")
        
        output.append("【计算结果】")
        output.append(f"安全泄放量 Ws: {W_s:.2f} kg/h")
        output.append(f"最小泄放面积: {result['flow_area']:.4f} {result['unit']}")
        output.append(f"流动状态: {result['flow_type']}")
        
        if 'critical_ratio' in result:
            output.append(f"临界压力比: {result['critical_ratio']:.4f}")
        if 'X_factor' in result:
            output.append(f"气体特性系数 X: {result['X_factor']:.2f}")
        if 'delta_P' in result:
            output.append(f"计算压差: {result['delta_P']:.4f} MPa")
        
        if result['warning']:
            output.append("")
            output.append(f"【警告】 {result['warning']}")
        
        output.append("")
        output.append("【计算依据GB150】")
        output.append("")
        output.append("=" * 60)
        
        self.result_text.insert(1.0, "\n".join(output))

'''
# 主应用程序类（如果需要）
class MainApplication:
    """主应用程序"""
    
    def __init__(self, root):
        self.root = root
        self.root.title("安全阀计算系统")
        self.root.geometry("400x200")
        
        self.create_widgets()
    
    def create_widgets(self):
        main_frame = ttk.Frame(self.root, padding="20")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        title_label = ttk.Label(main_frame, text="安全阀泄放面积计算系统", 
                               font=('Arial', 16, 'bold'))
        title_label.grid(row=0, column=0, pady=20)
        
        # 打开计算对话框的按钮
        calc_button = ttk.Button(main_frame, text="打开安全阀计算", 
                                command=self.open_safety_valve_calc)
        calc_button.grid(row=1, column=0, pady=10)
        
        # 退出按钮
        exit_button = ttk.Button(main_frame, text="退出", 
                                command=self.root.quit)
        exit_button.grid(row=2, column=0, pady=10)
        
        # 配置权重
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(0, weight=1)
    
    def open_safety_valve_calc(self):
        """打开安全阀计算对话框"""
        SafetyValveDialog(self.root)

if __name__ == "__main__":
    root = tk.Tk()
    app = MainApplication(root)
    root.mainloop()
'''