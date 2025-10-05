import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
import math
try:
    from CoolProp.CoolProp import PropsSI
    coolprop_available = True
except ImportError:
    coolprop_available = False


class PipeResistanceDialog(tk.Toplevel):
    def __init__(self, parent):
        super().__init__(parent)
        self.title("管道阻力计算")
        self.geometry("650x600")
        self.transient(parent)
        self.grab_set()
        
        # 定义管子类别及其绝对粗糙度(单位：m)，1mm = 0.001m
        self.pipe_types = {
            "无缝黄铜、铜及铅管": 0.01 / 1000,    # 0.01mm转换为m
            "操作中基本无腐蚀的无缝钢管": 0.1 / 1000,  # 0.1mm转换为m
            "操作中有轻度腐蚀的无缝钢管": 0.2 / 1000,  # 0.2mm转换为m
            "操作中有显著腐蚀的无缝钢管": 0.5 / 1000,  # 0.5mm转换为m
            "钢板卷管": 0.33 / 1000,          # 0.33mm转换为m
            "铸铁管": 0.85 / 1000,            # 0.85mm转换为m
            "干净的玻璃管": 0.01 / 1000       # 0.01mm转换为m
        }
        
        # 定义管道附件及其局部阻力系数
        self.fittings = {
            "标准90°弯头": 0.75,
            "标准45°弯头": 0.35,
            "长半径90°弯头": 0.45,
            "长半径45°弯头": 0.2,
            "180°弯头": 1.5,
            "三通(主管流)": 0.1,
            "三通(支流)": 1.0,
            "闸阀(全开)": 0.17,
            "闸阀(半开)": 4.5,
            "球阀(全开)": 0.05,
            "截止阀(全开)": 6.0,
            "截止阀(半开)": 12.0,
            "止回阀(旋启式)": 2.0,
            "止回阀(升降式)": 10.0,
            "角阀(全开)": 3.00,
            "底阀": 15.00
        }
        
        # 常用流体列表（CoolProp支持的流体）
        self.fluids = [
            "水", "空气", "二氧化碳", "甲烷", "乙烷", "丙烷", 
            "甲醇", "乙醇", "氨", "氢气", "氮气", "氧气"
        ]
        # CoolProp流体名称映射
        self.fluid_mapping = {
            "水": "Water",
            "空气": "Air",
            "二氧化碳": "CO2",
            "甲烷": "Methane",
            "乙烷": "Ethane",
            "丙烷": "Propane",
            "甲醇": "Methanol",
            "乙醇": "Ethanol",
            "氨": "Ammonia",
            "氢气": "Hydrogen",
            "氮气": "Nitrogen",
            "氧气": "Oxygen"
        }
        
        # 创建 Notebook 组件管理多个标签页
        self.notebook = ttk.Notebook(self)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 创建各个标签页
        self.pipe_fittings_frame = ttk.Frame(self.notebook)
        self.fluid_frame = ttk.Frame(self.notebook)
        self.result_frame = ttk.Frame(self.notebook)
        
        self.notebook.add(self.pipe_fittings_frame, text="管道参数与附件")
        self.notebook.add(self.fluid_frame, text="流体参数")
        self.notebook.add(self.result_frame, text="计算结果")
        
        # 绑定标签页切换事件
        self.notebook.bind("<<NotebookTabChanged>>", self.on_tab_changed)
        
        # 存储输入数据的变量
        self.data = {
            # 管道参数
            "pipe_type": tk.StringVar(value="操作中基本无腐蚀的无缝钢管"),
            "pipe_length": tk.StringVar(value="100"),  # 管道长度，m
            "pipe_diameter": tk.StringVar(value="0.1"),  # 管道内径，m
            "pipe_roughness": tk.StringVar(value=str(self.pipe_types["操作中基本无腐蚀的无缝钢管"])),
            
            # 流体参数
            "fluid_type": tk.StringVar(value="水"),  # 流体类型
            "temperature": tk.StringVar(value="20"),  # 温度，℃
            "pressure": tk.StringVar(value="0.101325"),  # 压力，MPa
            "fluid_density": tk.StringVar(value="1000"),  # 流体密度，kg/m³
            "fluid_viscosity": tk.StringVar(value="0.001"),  # 动力粘度，Pa·s
            "flow_type": tk.StringVar(value="体积流量"),  # 流量类型
            "volume_flow": tk.StringVar(value="180"),  # 体积流量，m³/h
            "mass_flow": tk.StringVar(value="180000")  # 质量流量，kg/h
        }
        
        # 存储管道附件数量的字典
        self.fittings_counts = {fitting: tk.StringVar(value="0") for fitting in self.fittings.keys()}
        
        self.create_pipe_fittings_tab()
        self.create_fluid_tab()
        self.create_result_tab()
        
        # 绑定管道类型变化事件
        self.data["pipe_type"].trace_add("write", self.update_roughness)
        
        # 绑定流量类型变化事件
        self.data["flow_type"].trace_add("write", self.update_flow_visibility)
        
        # 如果CoolProp可用，绑定流体类型、温度、压力变化事件
        if coolprop_available:
            self.data["fluid_type"].trace_add("write", self.update_fluid_properties)
            self.data["temperature"].trace_add("write", self.update_fluid_properties)
            self.data["pressure"].trace_add("write", self.update_fluid_properties)
            # 初始更新一次流体属性
            self.update_fluid_properties()
        
        # 初始更新流量输入框可见性
        self.update_flow_visibility()
    
    def update_roughness(self, *args):
        """当管道类型改变时，更新绝对粗糙度"""
        pipe_type = self.data["pipe_type"].get()
        if pipe_type in self.pipe_types:
            self.data["pipe_roughness"].set(str(self.pipe_types[pipe_type]))
    
    def update_flow_visibility(self, *args):
        """根据选择的流量类型显示/隐藏对应的输入框"""
        flow_type = self.data["flow_type"].get()
        if flow_type == "体积流量":
            self.volume_flow_frame.pack(fill=tk.X, padx=5, pady=5)
            self.mass_flow_frame.pack_forget()
        else:  # 质量流量
            self.volume_flow_frame.pack_forget()
            self.mass_flow_frame.pack(fill=tk.X, padx=5, pady=5)
    
    def update_fluid_properties(self, *args):
        """使用CoolProp更新流体密度和粘度"""
        if not coolprop_available:
            return
            
        try:
            fluid_name = self.data["fluid_type"].get()
            if fluid_name not in self.fluid_mapping:
                return
                
            coolprop_name = self.fluid_mapping[fluid_name]
            
            # 转换单位：℃ -> K，MPa -> Pa
            T_celsius = float(self.data["temperature"].get())
            T = T_celsius + 273.15  # 转换为开尔文
            
            P_mpa = float(self.data["pressure"].get())
            P = P_mpa * 1e6  # 转换为帕斯卡
            
            # 获取密度 (kg/m³)
            density = PropsSI('D', 'T', T, 'P', P, coolprop_name)
            # 获取动力粘度 (Pa·s)
            viscosity = PropsSI('V', 'T', T, 'P', P, coolprop_name)
            
            # 更新输入框
            self.data["fluid_density"].set(f"{density:.2f}")
            self.data["fluid_viscosity"].set(f"{viscosity:.6f}")
            
        except Exception as e:
            # 忽略计算错误，保持现有值
            pass
    
    def create_pipe_fittings_tab(self):
        # 创建主框架，管道参数在上，管道附件在下
        main_frame = ttk.Frame(self.pipe_fittings_frame)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 上部：管道基本参数
        pipe_frame = ttk.LabelFrame(main_frame, text="管道基本参数")
        pipe_frame.pack(fill=tk.X, padx=5, pady=5)
        
        # 管道类型
        row = ttk.Frame(pipe_frame)
        row.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(row, text="管道类型:", width=25, anchor=tk.W).pack(side=tk.LEFT)
        
        pipe_type_combo = ttk.Combobox(row, textvariable=self.data["pipe_type"], 
                                      values=list(self.pipe_types.keys()), width=40)
        pipe_type_combo.pack(side=tk.LEFT, padx=5)
        
        # 管道长度
        self.create_input_row(pipe_frame, "管道长度 (m):", "pipe_length", 
                             "管道总长度，包括所有直管段")
        
        # 管道内径
        self.create_input_row(pipe_frame, "管道内径 (m):", "pipe_diameter", 
                             "管道公称直径对应的内径")
        
        # 管道粗糙度（根据管道类型自动填充）
        self.create_input_row(pipe_frame, "绝对粗糙度 (m):", "pipe_roughness", 
                             "根据管道类型自动计算")
        
        # 下部：管道附件（两列显示）
        fittings_frame = ttk.LabelFrame(main_frame, text="管道附件 (局部阻力元件)")
        fittings_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 创建滚动区域放置所有附件
        canvas = tk.Canvas(fittings_frame)
        scrollbar = ttk.Scrollbar(fittings_frame, orient="vertical", command=canvas.yview)
        scrollable_frame = ttk.Frame(canvas)
        
        scrollable_frame.bind(
            "<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all"))
        )
        
        canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
        canvas.configure(yscrollcommand=scrollbar.set)
        
        # 添加所有管道附件（两列布局）
        fitting_items = list(self.fittings.items())
        cols = 2  # 两列布局
        for i, (fitting, coeff) in enumerate(fitting_items):
            row_frame = ttk.Frame(scrollable_frame)
            row_frame.grid(row=i//cols, column=i%cols, padx=10, pady=3, sticky=tk.W)
            
            ttk.Label(row_frame, text=f"{fitting}(个):", width=20, anchor=tk.W).pack(side=tk.LEFT)
            ttk.Entry(row_frame, textvariable=self.fittings_counts[fitting], width=5).pack(side=tk.LEFT, padx=5)
            ttk.Label(row_frame, text=f"(ξ={coeff})", font=("SimHei", 9), 
                     foreground="#666666").pack(side=tk.LEFT)
        
        # 布局滚动区域
        canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        
        # 按钮框架
        btn_frame = ttk.Frame(self.pipe_fittings_frame)
        btn_frame.pack(fill=tk.X, padx=10, pady=10)
        
        next_btn = ttk.Button(btn_frame, text="下一步", 
                             command=lambda: self.notebook.select(1))
        next_btn.pack(side=tk.RIGHT)
    
    def create_fluid_tab(self):
        # 创建流体参数输入表单
        fluid_type_frame = ttk.LabelFrame(self.fluid_frame, text="流体类型与状态")
        fluid_type_frame.pack(fill=tk.X, padx=10, pady=10)
        
        # 流体类型
        row = ttk.Frame(fluid_type_frame)
        row.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(row, text="流体类型:", width=25, anchor=tk.W).pack(side=tk.LEFT)
        
        fluid_combo = ttk.Combobox(row, textvariable=self.data["fluid_type"], 
                                  values=self.fluids, width=20)
        fluid_combo.pack(side=tk.LEFT, padx=5)
        
        if not coolprop_available:
            ttk.Label(row, text="(CoolProp未安装，无法自动获取参数)", 
                     font=("SimHei", 9), foreground="#ff0000").pack(side=tk.LEFT)
        
        # 温度（℃）
        self.create_input_row(fluid_type_frame, "温度 (℃):", "temperature", 
                             "流体温度，默认20℃")
        
        # 压力（MPa）
        self.create_input_row(fluid_type_frame, "压力 (MPa):", "pressure", 
                             "流体压力，默认0.101325MPa (标准大气压)")
        
        # 流体物理性质
        prop_frame = ttk.LabelFrame(self.fluid_frame, text="流体物理性质")
        prop_frame.pack(fill=tk.X, padx=10, pady=10)
        
        # 流体密度
        self.create_input_row(prop_frame, "流体密度 (kg/m³):", "fluid_density", 
                             "流体在操作温度和压力下的密度")
        
        # 流体动力粘度
        self.create_input_row(prop_frame, "流体动力粘度 (Pa·s):", "fluid_viscosity", 
                             "流体在操作温度和压力下的动力粘度")
        
        # 流量参数
        flow_frame = ttk.LabelFrame(self.fluid_frame, text="流量参数")
        flow_frame.pack(fill=tk.X, padx=10, pady=10)
        
        # 流量类型选择
        row = ttk.Frame(flow_frame)
        row.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(row, text="流量类型:", width=25, anchor=tk.W).pack(side=tk.LEFT)
        
        flow_type_frame = ttk.Frame(row)
        flow_type_frame.pack(side=tk.LEFT, padx=5)
        
        ttk.Radiobutton(flow_type_frame, text="体积流量", 
                       variable=self.data["flow_type"], value="体积流量").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(flow_type_frame, text="质量流量", 
                       variable=self.data["flow_type"], value="质量流量").pack(side=tk.LEFT, padx=5)
        
        # 体积流量输入框（m³/h）
        self.volume_flow_frame = ttk.Frame(flow_frame)
        self.create_input_row(self.volume_flow_frame, "体积流量 (m³/h):", "volume_flow", 
                             "流体在管道中的体积流量")
        
        # 质量流量输入框（kg/h）
        self.mass_flow_frame = ttk.Frame(flow_frame)
        self.create_input_row(self.mass_flow_frame, "质量流量 (kg/h):", "mass_flow", 
                             "流体在管道中的质量流量")
        
        # 按钮框架
        btn_frame = ttk.Frame(self.fluid_frame)
        btn_frame.pack(fill=tk.X, padx=10, pady=20)
        
        prev_btn = ttk.Button(btn_frame, text="上一步", 
                             command=lambda: self.notebook.select(0))
        prev_btn.pack(side=tk.LEFT)
        
        calc_btn = ttk.Button(btn_frame, text="计算阻力", 
                             command=self.calculate_resistance)
        calc_btn.pack(side=tk.RIGHT)
    
    def create_result_tab(self):
        # 创建结果显示区域
        frame = ttk.LabelFrame(self.result_frame, text="计算结果")
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 结果文本区域
        self.result_text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, 
                                                    width=90, height=30,
                                                    font=("SimHei", 10))
        self.result_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.result_text.config(state=tk.DISABLED)
        
        # 按钮框架
        btn_frame = ttk.Frame(self.result_frame)
        btn_frame.pack(fill=tk.X, padx=10, pady=10)
        
        recalc_btn = ttk.Button(btn_frame, text="重新计算", 
                               command=lambda: self.notebook.select(0))
        recalc_btn.pack(side=tk.LEFT)
        
        close_btn = ttk.Button(btn_frame, text="关闭", command=self.destroy)
        close_btn.pack(side=tk.RIGHT)
    
    def create_input_row(self, parent, label_text, var_name, tooltip_text):
        frame = ttk.Frame(parent)
        frame.pack(fill=tk.X, padx=5, pady=5)
        
        label = ttk.Label(frame, text=label_text, width=25, anchor=tk.W)
        label.pack(side=tk.LEFT)
        
        entry = ttk.Entry(frame, textvariable=self.data[var_name], width=20)
        entry.pack(side=tk.LEFT, padx=5)
        
        tooltip = ttk.Label(frame, text=tooltip_text, font=("SimHei", 9), 
                          foreground="#666666", wraplength=300)
        tooltip.pack(side=tk.LEFT)
    
    def on_tab_changed(self, event):
        # 如果切换到结果标签页且尚未计算，则提示用户先进行计算
        if self.notebook.index("current") == 2:
            self.result_text.config(state=tk.NORMAL)
            if self.result_text.get("1.0", tk.END).strip() == "":
                self.result_text.insert(tk.END, "请先在管道参数与附件和流体参数标签页中输入数据，然后点击计算按钮。")
            self.result_text.config(state=tk.DISABLED)
    
    def calculate_resistance(self):
        try:
            # 从输入字段获取数据并转换为数值
            pipe_type = self.data["pipe_type"].get()
            L = float(self.data["pipe_length"].get())  # 管道长度，m
            D = float(self.data["pipe_diameter"].get())  # 管道内径，m
            e = float(self.data["pipe_roughness"].get())  # 管道粗糙度，m
            
            fluid_type = self.data["fluid_type"].get()
            
            # 转换温度单位：℃ -> K
            T_celsius = float(self.data["temperature"].get())
            T = T_celsius + 273.15  # 开尔文
            
            # 转换压力单位：MPa -> Pa
            P_mpa = float(self.data["pressure"].get())
            P = P_mpa * 1e6  # 帕斯卡
            
            rho = float(self.data["fluid_density"].get())  # 流体密度，kg/m³
            mu = float(self.data["fluid_viscosity"].get())  # 动力粘度，Pa·s
            
            # 处理流量输入（转换为m³/s用于计算）
            flow_type = self.data["flow_type"].get()
            if flow_type == "体积流量":
                # 体积流量：m³/h -> m³/s
                Q_m3h = float(self.data["volume_flow"].get())
                Q = Q_m3h / 3600  # 转换为m³/s
                mass_flow_kgh = Q_m3h * rho  # 计算对应的质量流量
            else:
                # 质量流量：kg/h -> m³/s（通过密度转换）
                mass_flow_kg = float(self.data["mass_flow"].get())
                mass_flow_kgh = mass_flow_kg
                Q = (mass_flow_kg / 3600) / rho  # 转换为m³/s
                Q_m3h = Q * 3600  # 计算对应的体积流量
            
            # 验证输入值是否合理
            if (D <= 0 or L <= 0 or e < 0 or rho <= 0 or mu <= 0 or 
                Q <= 0 or P_mpa <= 0):
                messagebox.showerror("输入错误", "所有参数必须为正数！")
                return
            
            # 获取所有管道附件的数量并计算总局部阻力系数
            total_local_coeff = 0.0
            fittings_data = []
            for fitting, coeff in self.fittings.items():
                count = int(self.fittings_counts[fitting].get())
                if count > 0:
                    total_local_coeff += count * coeff
                    fittings_data.append((fitting, count, coeff, count * coeff))
            
            if total_local_coeff <= 0:
                messagebox.showinfo("提示", "未选择任何管道附件，局部阻力将为0")
            
            # 计算流速
            A = math.pi * (D / 2) ** 2  # 管道横截面积
            v = Q / A  # 流速，m/s
            
            # 计算雷诺数
            Re = (rho * v * D) / mu  # 雷诺数
            
            # 根据SH/T3035-2018计算沿程阻力系数λ
            if Re < 2000:  # 层流
                lambda_ = 64 / Re
                flow_type_text = "层流"
            else:  # 湍流，使用Colebrook-White公式
                # 使用迭代法求解Colebrook-White方程
                lambda_ = 0.02  # 初始猜测值
                for _ in range(50):  # 迭代次数
                    prev_lambda = lambda_
                    sqrt_lambda = math.sqrt(lambda_)
                    term1 = 1 / sqrt_lambda
                    term2 = 2 * math.log10((e / D) / 3.7 + 2.51 / (Re * sqrt_lambda))
                    lambda_ = (1 / (-term2)) **2
                    if abs(lambda_ - prev_lambda) < 1e-8:
                        break
                flow_type_text = "湍流"
            
            # 计算沿程阻力损失 (Pa)
            h_f = lambda_ * (L / D) * (rho * v** 2) / 2
            
            # 计算局部阻力损失 (Pa)
            h_j = total_local_coeff * (rho * v **2) / 2
            
            # 总阻力损失
            h_total = h_f + h_j
            
            # 转换为其他常用单位
            h_total_m = h_total / (rho * 9.81)  # 米水柱
            h_total_kgf = h_total / 9.80665  # kgf/m²
            h_total_mpa = h_total / 1e6  # MPa
            
            # 显示结果
            self.result_text.config(state=tk.NORMAL)
            self.result_text.delete("1.0", tk.END)
            
            result_str = f"管道阻力计算结果 (依据SH/T3035-2018)\n"
            result_str += f"=========================================\n\n"
            
            result_str += f"管道参数:\n"
            result_str += f"  管道类型: {pipe_type}\n"
            result_str += f"  管道长度: {L:.2f} m\n"
            result_str += f"  管道内径: {D:.4f} m\n"
            result_str += f"  绝对粗糙度: {e:.6f} m ({e*1000:.3f} mm)\n\n"
            
            result_str += f"流体参数:\n"
            result_str += f"  流体类型: {fluid_type}\n"
            result_str += f"  温度: {T_celsius:.2f} ℃ ({T:.2f} K)\n"
            result_str += f"  压力: {P_mpa:.6f} MPa ({P:.2f} Pa)\n"
            result_str += f"  密度: {rho:.2f} kg/m³\n"
            result_str += f"  动力粘度: {mu:.6f} Pa·s\n\n"
            
            result_str += f"流动参数:\n"
            result_str += f"  体积流量: {Q_m3h:.2f} m³/h ({Q:.6f} m³/s)\n"
            result_str += f"  质量流量: {mass_flow_kgh:.2f} kg/h\n"
            result_str += f"  流速: {v:.4f} m/s\n"
            result_str += f"  雷诺数: {Re:.2f} ({flow_type_text})\n"
            result_str += f"  沿程阻力系数λ: {lambda_:.6f}\n\n"
            
            result_str += f"阻力损失计算:\n"
            result_str += f"  沿程阻力损失: {h_f:.2f} Pa ({h_f/1e6:.6f} MPa)\n"
            result_str += f"  局部阻力损失: {h_j:.2f} Pa ({h_j/1e6:.6f} MPa)\n"
            result_str += f"  总阻力损失: {h_total:.2f} Pa ({h_total_mpa:.6f} MPa)\n"
            result_str += f"  总阻力损失: {h_total_m:.4f} m 水柱\n"
            result_str += f"  总阻力损失: {h_total_kgf:.2f} kgf/m²\n\n"
            
            result_str += f"局部阻力组成:\n"
            result_str += f"  附件名称         数量   单个ξ值   总ξ值\n"
            result_str += f"  ----------------------------------------\n"
            for fitting, count, coeff, total in fittings_data:
                result_str += f"  {fitting:14s} {count:4d}    {coeff:6.2f}    {total:6.2f}\n"
            result_str += f"  ----------------------------------------\n"
            result_str += f"  总局部阻力系数: {total_local_coeff:.2f}\n"
            
            self.result_text.insert(tk.END, result_str)
            self.result_text.config(state=tk.DISABLED)
            
            # 切换到结果标签页
            self.notebook.select(2)
            
        except ValueError as ve:
            messagebox.showerror("输入错误", f"请输入有效的数值: {str(ve)}")
        except Exception as e:
            messagebox.showerror("计算错误", f"计算过程中发生错误: {str(e)}")
    