import tkinter as tk
from tkinter import ttk, messagebox
import math

try:
    import CoolProp.CoolProp as CP
    COOLPROP_AVAILABLE = True
except ImportError:
    COOLPROP_AVAILABLE = False
    print("警告: CoolProp未安装，将使用手动输入模式")

class PumpPowerCalculator:
    """泵轴功率计算核心类"""
    
    def __init__(self):
        self.fluid_name = "Water"
        self.density = 1000  # kg/m³, 水的默认密度
        
    def get_fluid_density(self, fluid_name=None, temperature=20, pressure=0.101325):
        """
        通过CoolProp获取流体密度
        
        参数:
            fluid_name: 流体名称
            temperature: 温度 (°C)
            pressure: 压力 (MPa)
        """
        if not COOLPROP_AVAILABLE:
            return self.density
            
        try:
            if fluid_name:
                self.fluid_name = fluid_name
            # 将压力从MPa转换为Pa (1 MPa = 1e6 Pa)
            pressure_pa = pressure * 1e6
            # 温度从°C转换为K
            temperature_k = temperature + 273.15
            
            # 检查流体是否在CoolProp中可用
            try:
                CP.PropsSI('D', 'T', temperature_k, 'P', pressure_pa, self.fluid_name)
            except ValueError:
                return None  # 流体不存在
                
            density = CP.PropsSI('D', 'T', temperature_k, 'P', pressure_pa, self.fluid_name)
            return density
        except Exception as e:
            print(f"CoolProp获取密度失败: {e}")
            return None
    
    def calculate_shaft_power(self, flow_rate, head, efficiency, density, is_mass_flow=False):
        """
        计算泵的轴功率
        
        参数:
            flow_rate: 流量 (m³/h 或 kg/h)
            head: 扬程 (m)
            efficiency: 效率 (0-1)
            density: 密度 (kg/m³)
            is_mass_flow: 是否为质量流量
            
        返回:
            shaft_power: 轴功率 (kW)
        """
        if efficiency <= 0 or efficiency > 1:
            raise ValueError("效率必须在0-1之间")
            
        if flow_rate <= 0 or head <= 0 or density <= 0:
            raise ValueError("流量、扬程和密度必须大于0")
        
        if is_mass_flow:
            # 质量流量 kg/h 转换为 kg/s
            mass_flow_kgs = flow_rate / 3600
            # 轴功率计算公式: P = (m * g * H) / (1000 * η)
            g = 9.81  # 重力加速度 m/s²
            hydraulic_power = mass_flow_kgs * g * head  # 水力功率 W
        else:
            # 体积流量 m³/h 转换为 m³/s
            flow_rate_m3s = flow_rate / 3600
            # 轴功率计算公式: P = (ρ * g * Q * H) / (1000 * η)
            g = 9.81  # 重力加速度 m/s²
            hydraulic_power = density * g * flow_rate_m3s * head  # 水力功率 W
        
        shaft_power = hydraulic_power / (efficiency * 1000)  # 轴功率 kW
        
        return shaft_power
    
    def calculate_hydraulic_power(self, flow_rate, head, density, is_mass_flow=False):
        """计算水力功率"""
        if is_mass_flow:
            # 质量流量 kg/h 转换为 kg/s
            mass_flow_kgs = flow_rate / 3600
            g = 9.81
            hydraulic_power = mass_flow_kgs * g * head / 1000  # kW
        else:
            # 体积流量 m³/h 转换为 m³/s
            flow_rate_m3s = flow_rate / 3600
            g = 9.81
            hydraulic_power = density * g * flow_rate_m3s * head / 1000  # kW
        return hydraulic_power

class PumpCalculatorDialog(tk.Toplevel):
    """泵功率计算对话框"""
    
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent
        self.calculator = PumpPowerCalculator()
        
        # 流体名称映射：中文显示名称 -> CoolProp英文名称
        self.fluid_mapping = {
            "水": "Water",
            "乙醇": "Ethanol",
            "甲醇": "Methanol",
            "丙烷": "Propane",
            "丁烷": "Butane",
            "异丙醇": "Isopropanol",
            "氨水": "Ammonia",
            "R134a制冷剂": "R134a",
            "R22制冷剂": "R22",
            "R410A制冷剂": "R410A",
            "R404A制冷剂": "R404A",
            "R507A制冷剂": "R507A",
            "乙二醇": "EthyleneGlycol",
            "丙二醇": "PropyleneGlycol",
            "甲苯": "Toluene",
            "苯": "Benzene",
            "辛烷": "Octane",
            "癸烷": "Decane",
            "十二烷": "Dodecane",
            "丙酮": "Acetone",
            "乙酸": "AceticAcid",
            "乙酸甲酯": "MethylAcetate",
            "乙酸乙酯": "EthylAcetate",
            "硫酸": "SulfuricAcid",
            "硝酸": "NitricAcid",
            "盐酸": "HydrochloricAcid",
            "正庚烷": "n-Heptane",
            "正己烷": "n-Hexane",
            "环己烷": "Cyclohexane",
            "二氧化碳": "CarbonDioxide",
            "氮气": "Nitrogen",
            "氧气": "Oxygen",
            "氩气": "Argon",
            "氦气": "Helium",
            "氢气": "Hydrogen",
            "甲烷": "Methane",
            "乙烷": "Ethane"
        }
        
        self.title("泵轴功率计算器")
        self.geometry("600x850")
        self.resizable(True, True)
        
        # 设置样式
        self.style = ttk.Style()
        self.style.configure('TFrame', background='#f0f0f0')
        self.style.configure('TLabel', background='#f0f0f0', font=('Arial', 10))
        self.style.configure('TButton', font=('Arial', 10))
        self.style.configure('Header.TLabel', font=('Arial', 12, 'bold'))
        self.style.configure('Warning.TLabel', foreground='red', font=('Arial', 9))
        
        self.create_widgets()
        self.center_window()
        
    def center_window(self):
        """窗口居中显示"""
        self.update_idletasks()
        width = self.winfo_width()
        height = self.winfo_height()
        x = (self.winfo_screenwidth() // 2) - (width // 2)
        y = (self.winfo_screenheight() // 2) - (height // 2)
        self.geometry(f'{width}x{height}+{x}+{y}')
    
    def create_widgets(self):
        """创建界面组件"""
        main_frame = ttk.Frame(self, padding="20")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 标题
        title_label = ttk.Label(main_frame, text="泵轴功率计算器", 
                               style='Header.TLabel')
        title_label.pack(pady=(0, 20))
        
        # 流体属性框架
        fluid_frame = ttk.LabelFrame(main_frame, text="流体属性", padding="10")
        fluid_frame.pack(fill=tk.X, pady=(0, 10))
        
        # CoolProp流体选择
        if COOLPROP_AVAILABLE:
            ttk.Label(fluid_frame, text="流体:").grid(row=0, column=0, sticky=tk.W, pady=5)
            self.fluid_display_var = tk.StringVar(value="水")
            
            # 使用中文显示名称
            chinese_fluid_names = list(self.fluid_mapping.keys())
            
            fluid_combo = ttk.Combobox(fluid_frame, textvariable=self.fluid_display_var,
                                      values=chinese_fluid_names, state="readonly")
            fluid_combo.grid(row=0, column=1, sticky=tk.W+tk.E, pady=5, padx=(10, 0))
            fluid_combo.bind('<<ComboboxSelected>>', self.on_fluid_change)
            
            ttk.Label(fluid_frame, text="温度 (°C):").grid(row=1, column=0, sticky=tk.W, pady=5)
            self.temp_var = tk.StringVar(value="20")
            temp_entry = ttk.Entry(fluid_frame, textvariable=self.temp_var, width=15)
            temp_entry.grid(row=1, column=1, sticky=tk.W, pady=5, padx=(10, 0))
            
            ttk.Label(fluid_frame, text="压力 (MPa)(绝压):").grid(row=2, column=0, sticky=tk.W, pady=5)
            self.pressure_var = tk.StringVar(value="0.101325")
            pressure_entry = ttk.Entry(fluid_frame, textvariable=self.pressure_var, width=15)
            pressure_entry.grid(row=2, column=1, sticky=tk.W, pady=5, padx=(10, 0))
            
            ttk.Button(fluid_frame, text="获取密度", 
                      command=self.get_density_from_coolprop).grid(row=3, column=0, columnspan=2, pady=10)
            
            # CoolProp警告标签
            self.coolprop_warning_var = tk.StringVar()
            self.coolprop_warning_label = ttk.Label(fluid_frame, 
                                                   textvariable=self.coolprop_warning_var,
                                                   style='Warning.TLabel')
            self.coolprop_warning_label.grid(row=4, column=0, columnspan=2, sticky=tk.W, pady=(5, 0))
        
        # 手动密度输入
        ttk.Label(fluid_frame, text="密度 (kg/m³):").grid(row=5, column=0, sticky=tk.W, pady=5)
        self.density_var = tk.StringVar(value="1000")
        density_entry = ttk.Entry(fluid_frame, textvariable=self.density_var, width=15)
        density_entry.grid(row=5, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 常见液体密度参考
        density_info = ttk.Label(fluid_frame, 
                                text="常见液体密度参考: 水=1000, 乙醇=789, 机油=900, 海水=1025",
                                font=('Arial', 8), foreground='gray')
        density_info.grid(row=6, column=0, columnspan=2, sticky=tk.W, pady=(5, 0))
        
        fluid_frame.columnconfigure(1, weight=1)
        
        # 流量输入框架
        flow_frame = ttk.LabelFrame(main_frame, text="流量参数", padding="10")
        flow_frame.pack(fill=tk.X, pady=(0, 10))
        
        # 流量类型选择
        ttk.Label(flow_frame, text="流量类型:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.flow_type_var = tk.StringVar(value="volume")
        flow_type_frame = ttk.Frame(flow_frame)
        flow_type_frame.grid(row=0, column=1, sticky=tk.W+tk.E, pady=5, padx=(10, 0))
        
        ttk.Radiobutton(flow_type_frame, text="体积流量", variable=self.flow_type_var, 
                       value="volume", command=self.on_flow_type_change).pack(side=tk.LEFT)
        ttk.Radiobutton(flow_type_frame, text="质量流量", variable=self.flow_type_var, 
                       value="mass", command=self.on_flow_type_change).pack(side=tk.LEFT, padx=(20, 0))
        
        # 流量输入
        self.flow_label = ttk.Label(flow_frame, text="体积流量 (m³/h):")
        self.flow_label.grid(row=1, column=0, sticky=tk.W, pady=5)
        self.flow_var = tk.StringVar(value="100")
        flow_entry = ttk.Entry(flow_frame, textvariable=self.flow_var)
        flow_entry.grid(row=1, column=1, sticky=tk.W+tk.E, pady=5, padx=(10, 0))
        
        flow_frame.columnconfigure(1, weight=1)
        
        # 泵参数框架
        pump_frame = ttk.LabelFrame(main_frame, text="泵参数", padding="10")
        pump_frame.pack(fill=tk.X, pady=(0, 10))
        
        # 扬程输入
        ttk.Label(pump_frame, text="扬程 (m):").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.head_var = tk.StringVar(value="50")
        head_entry = ttk.Entry(pump_frame, textvariable=self.head_var)
        head_entry.grid(row=0, column=1, sticky=tk.W+tk.E, pady=5, padx=(10, 0))
        
        # 效率输入
        ttk.Label(pump_frame, text="效率 (0-1):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.efficiency_var = tk.StringVar(value="0.75")
        efficiency_entry = ttk.Entry(pump_frame, textvariable=self.efficiency_var)
        efficiency_entry.grid(row=1, column=1, sticky=tk.W+tk.E, pady=5, padx=(10, 0))
        
        # 效率参考
        efficiency_info = ttk.Label(pump_frame, 
                                  text="效率参考: 小型泵=0.5-0.7, 中型泵=0.7-0.85, 大型泵=0.85-0.95",
                                  font=('Arial', 8), foreground='gray')
        efficiency_info.grid(row=2, column=0, columnspan=2, sticky=tk.W, pady=(5, 0))
        
        pump_frame.columnconfigure(1, weight=1)
        
        # 计算按钮
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(fill=tk.X, pady=20)
        
        ttk.Button(button_frame, text="计算轴功率", 
                  command=self.calculate_power, style='TButton').pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(button_frame, text="清空输入", 
                  command=self.clear_inputs).pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(button_frame, text="退出", 
                  command=self.destroy).pack(side=tk.LEFT)
        
        # 结果显示框架
        result_frame = ttk.LabelFrame(main_frame, text="计算结果", padding="10")
        result_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 10))
        
        self.result_text = tk.Text(result_frame, height=12, width=60, font=('Arial', 10))
        scrollbar = ttk.Scrollbar(result_frame, orient=tk.VERTICAL, command=self.result_text.yview)
        self.result_text.configure(yscrollcommand=scrollbar.set)
        
        self.result_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 状态栏
        self.status_var = tk.StringVar()
        if not COOLPROP_AVAILABLE:
            self.status_var.set("状态: CoolProp未安装，请手动输入密度")
        else:
            self.status_var.set("状态: 就绪 - 选择流体并输入参数")
            
        status_label = ttk.Label(main_frame, textvariable=self.status_var, 
                                relief=tk.SUNKEN, style='TLabel')
        status_label.pack(fill=tk.X, pady=(5, 0))
        
        main_frame.columnconfigure(0, weight=1)
    
    def get_fluid_english_name(self, chinese_name):
        """获取流体对应的英文名称"""
        return self.fluid_mapping.get(chinese_name, "Water")
    
    def on_flow_type_change(self):
        """流量类型改变时更新标签"""
        if self.flow_type_var.get() == "volume":
            self.flow_label.config(text="体积流量 (m³/h):")
        else:
            self.flow_label.config(text="质量流量 (kg/h):")
    
    def on_fluid_change(self, event=None):
        """当流体类型改变时更新默认参数"""
        chinese_name = self.fluid_display_var.get()
        english_name = self.get_fluid_english_name(chinese_name)
        
        # 根据流体类型设置合适的默认温度和压力
        if english_name == "Water":
            self.temp_var.set("20")
            self.pressure_var.set("0.101325")
        elif english_name in ["Ethanol", "Methanol", "Isopropanol"]:
            self.temp_var.set("20")
            self.pressure_var.set("0.101325")
        elif "Oil" in english_name:
            self.temp_var.set("40")
            self.pressure_var.set("0.101325")
        elif english_name in ["R134a", "R22", "R410A"]:
            self.temp_var.set("5")
            self.pressure_var.set("0.5")
        elif english_name in ["Ammonia", "SulfuricAcid", "NitricAcid", "HydrochloricAcid"]:
            self.temp_var.set("25")
            self.pressure_var.set("0.101325")
        # 清除警告
        self.coolprop_warning_var.set("")
    
    def get_density_from_coolprop(self):
        """从CoolProp获取密度"""
        if not COOLPROP_AVAILABLE:
            messagebox.showwarning("警告", "CoolProp未安装，无法获取流体密度")
            return
            
        try:
            chinese_name = self.fluid_display_var.get()
            english_name = self.get_fluid_english_name(chinese_name)
            temperature = float(self.temp_var.get())
            pressure = float(self.pressure_var.get())
            
            density = self.calculator.get_fluid_density(english_name, temperature, pressure)
            
            if density is None:
                self.coolprop_warning_var.set(f"警告: 流体 '{chinese_name}' 在CoolProp中不存在，请手动输入密度")
                messagebox.showwarning("流体不存在", 
                    f"流体 '{chinese_name}' 在CoolProp数据库中不存在。\n"
                    f"请手动输入该流体的密度值。\n\n"
                    f"常见参考值:\n"
                    f"- 水: 1000 kg/m³\n"
                    f"- 乙醇: 789 kg/m³\n"
                    f"- 机油: 900 kg/m³\n"
                    f"- 海水: 1025 kg/m³")
            else:
                self.density_var.set(f"{density:.2f}")
                self.coolprop_warning_var.set("")
                self.status_var.set(f"状态: 成功获取 {chinese_name} 在 {temperature}°C, {pressure}MPa 时的密度")
            
        except ValueError:
            messagebox.showerror("错误", "请输入有效的温度和压力数值")
        except Exception as e:
            messagebox.showerror("错误", f"获取密度失败: {str(e)}")
    
    def calculate_power(self):
        """计算轴功率"""
        try:
            # 获取输入值
            flow_rate = float(self.flow_var.get())
            head = float(self.head_var.get())
            efficiency = float(self.efficiency_var.get())
            density = float(self.density_var.get())
            is_mass_flow = (self.flow_type_var.get() == "mass")
            
            # 验证输入
            if flow_rate <= 0:
                raise ValueError("流量必须大于0")
            if head <= 0:
                raise ValueError("扬程必须大于0")
            if efficiency <= 0 or efficiency > 1:
                raise ValueError("效率必须在0-1之间")
            if density <= 0 and not is_mass_flow:
                raise ValueError("密度必须大于0")
            
            # 计算功率
            shaft_power = self.calculator.calculate_shaft_power(flow_rate, head, efficiency, density, is_mass_flow)
            hydraulic_power = self.calculator.calculate_hydraulic_power(flow_rate, head, density, is_mass_flow)
            
            # 显示结果
            self.display_results(flow_rate, head, efficiency, density, 
                               hydraulic_power, shaft_power, is_mass_flow)
            
            self.status_var.set("状态: 计算完成")
            
        except ValueError as e:
            messagebox.showerror("输入错误", str(e))
        except Exception as e:
            messagebox.showerror("计算错误", f"计算过程中发生错误: {str(e)}")
    
    def get_standard_motor_power(self, required_power):
        """获取标准电机功率"""
        # 标准电机功率序列 (kW)
        standard_powers = [0.75, 1.1, 1.5, 2.2, 3, 4, 5.5, 7.5, 11, 15, 18.5, 22, 
                          30, 37, 45, 55, 75, 90, 110, 132, 160, 200, 250, 315]
        
        # 考虑10%安全系数
        power_with_safety = required_power * 1.1
        
        # 选择第一个大于等于安全功率的标准电机功率
        for motor_power in standard_powers:
            if motor_power >= power_with_safety:
                return motor_power, power_with_safety
        
        # 如果所有标准功率都小于需求，返回最大值
        return standard_powers[-1], power_with_safety
    
    def display_results(self, flow_rate, head, efficiency, density, 
                       hydraulic_power, shaft_power, is_mass_flow):
        """显示计算结果"""
        # 获取标准电机功率
        standard_motor_power, power_with_safety = self.get_standard_motor_power(shaft_power)
        
        # 计算流量换算
        if is_mass_flow:
            flow_rate_volume = flow_rate / density * 3600  # kg/h 转换为 m³/h
            flow_rate_info = f"质量流量: {flow_rate} kg/h (体积流量: {flow_rate_volume:.2f} m³/h)"
        else:
            flow_rate_mass = flow_rate * density  # m³/h 转换为 kg/h
            flow_rate_lps = flow_rate / 3.6  # m³/h 转换为 L/s
            flow_rate_info = f"体积流量: {flow_rate} m³/h ({flow_rate_lps:.1f} L/s, 质量流量: {flow_rate_mass:.0f} kg/h)"

        result_text = f"""
=== 泵功率计算结果 ===

输入参数:
  {flow_rate_info}
  扬程: {head} m
  效率: {efficiency:.3f} ({efficiency*100:.1f}%)
  密度: {density:.2f} kg/m³

计算结果:
  水力功率: {hydraulic_power:.3f} kW
  轴功率: {shaft_power:.3f} kW

选型建议:
  • 所需轴功率: {shaft_power:.2f} kW
  • 考虑10%安全系数: {power_with_safety:.2f} kW
  • 推荐电机功率: {standard_motor_power} kW

功率说明:
  • 水力功率: 流体实际获得的功率
  • 轴功率: 泵轴需要输入的功率（包含效率损失）
  • 效率损失: {(1-efficiency)*100:.1f}%
"""
        self.result_text.delete(1.0, tk.END)
        self.result_text.insert(1.0, result_text)
    
    def clear_inputs(self):
        """清空输入"""
        self.flow_var.set("")
        self.head_var.set("")
        self.efficiency_var.set("")
        self.density_var.set("1000")
        self.flow_type_var.set("volume")
        self.on_flow_type_change()
        if COOLPROP_AVAILABLE:
            self.temp_var.set("20")
            self.pressure_var.set("0.101325")
            self.fluid_display_var.set("水")
            self.coolprop_warning_var.set("")
        self.result_text.delete(1.0, tk.END)
        self.status_var.set("状态: 输入已清空")
'''
class MainApplication:
    """主应用程序"""
    
    def __init__(self, root):
        self.root = root
        self.root.title("泵功率计算系统")
        self.root.geometry("300x150")
        
        # 主界面
        main_frame = ttk.Frame(root, padding="30")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        ttk.Label(main_frame, text="泵轴功率计算系统", 
                 font=('Arial', 14, 'bold')).pack(pady=(0, 20))
        
        ttk.Button(main_frame, text="打开计算器", 
                  command=self.open_calculator, width=20).pack(pady=10)
        
        ttk.Button(main_frame, text="退出系统", 
                  command=self.root.quit, width=20).pack(pady=10)
        
        self.center_window()
    
    def center_window(self):
        """窗口居中"""
        self.root.update_idletasks()
        width = self.root.winfo_width()
        height = self.root.winfo_height()
        x = (self.root.winfo_screenwidth() // 2) - (width // 2)
        y = (self.root.winfo_screenheight() // 2) - (height // 2)
        self.root.geometry(f'{width}x{height}+{x}+{y}')
    
    def open_calculator(self):
        """打开计算器对话框"""
        dialog = PumpCalculatorDialog(self.root)
        dialog.grab_set()  # 模态对话框

def main():
    """主函数"""
    root = tk.Tk()
    app = MainApplication(root)
    root.mainloop()

if __name__ == "__main__":
    main()
'''