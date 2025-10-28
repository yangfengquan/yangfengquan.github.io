
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import os
import json
import webbrowser
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import CoolProp.CoolProp as CP
from Dialog import MaterialDialog
from FluidPipeAnalyzer import FluidPipeAnalyzer
from Activation import RegistryStorage, HardwareActivation

class PipeAnalysisApp:
    """管道分析主应用程序"""
    
    def __init__(self, root):
        self.root = root
        self.analyzer = FluidPipeAnalyzer()
        """设置GUI界面"""
        self.root.title("管道分析软件")
        self.root.geometry("1200x800")
        # 加载材料数据
        #self.analyzer.material_manager.load_materials_from_file()
   
        # 初始化注册表存储
        self.registry_storage = RegistryStorage("YPipe")
        
        # 初始化激活器
        self.activator = HardwareActivation(secret_key="Ypipe2025")
        
        # 检查软件状态
        self.check_software_status()

        #self.setup_gui()

    def check_software_status(self):
        """检查软件是已激活、试用中还是已过期"""
        # 检查是否已激活
        if self.is_activated():
            self.setup_gui()
            return
            
        # 检查试用次数（从注册表获取）
        trial_count = self.registry_storage.get_trial_count()
        if trial_count < 5:
            # 还有试用次数
            self.registry_storage.update_trial_count(trial_count + 1)
            if trial_count > 2:
                messagebox.showinfo("试用提示", 
                                f"这是您的第 {trial_count + 1} 次试用，还剩 {4 - trial_count} 次试用机会。\n"
                                f"试用结束后,添加QQ群：816103114，免费获取激活码。")
            self.setup_gui()
        else:
            # 试用次数已用完，显示激活界面
            self.show_activation_interface()
    
    def is_activated(self):
        """检查软件是否已激活"""
        try:
            activation_data = self.registry_storage.get_activation_data()
            if activation_data and activation_data.get("activated", False):
                # 验证保存的激活码是否有效
                request_code = activation_data.get("request_code")
                activation_code = activation_data.get("activation_code")
                return self.activator.verify_activation(activation_code, request_code)
        except:
            pass
        return False
    
    def save_activation_status(self, request_code, activation_code):
        """保存激活状态到注册表"""
        activation_data = {
            "activated": True,
            "request_code": request_code,
            "activation_code": activation_code,
            #"timestamp": str(os.path.getmtime(__file__))
        }
        return self.registry_storage.save_activation_data(activation_data)

    def show_activation_interface(self):
        """显示激活界面"""
        # 清空当前界面
        for widget in self.root.winfo_children():
            widget.destroy()
        
        # 激活界面标题
        ttk.Label(self.root, text="软件激活", style="Header.TLabel").pack(pady=20)
        
        # 激活框架
        activation_frame = ttk.Frame(self.root, padding="20")
        activation_frame.pack(fill=tk.BOTH, expand=True)
        
        # 申请码区域
        ttk.Label(activation_frame, text="您的申请码:").pack(anchor=tk.W, pady=(10, 5))
        
        request_code = self.activator.generate_request_code()
        self.request_code_var = tk.StringVar(value=request_code)

        request_entry = ttk.Entry(activation_frame, textvariable=self.request_code_var, 
                                 state="readonly", width=50)
        request_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(activation_frame, text="复制申请码", 
          command=lambda: [self.root.clipboard_clear(), self.root.clipboard_append(request_code)]).pack(anchor=tk.W, pady=(0, 15))
        
        # 激活码区域
        ttk.Label(activation_frame, text="请输入激活码:").pack(anchor=tk.W, pady=(10, 5))
        
        self.activation_code_var = tk.StringVar()
        activation_entry = ttk.Entry(activation_frame, textvariable=self.activation_code_var, width=50)
        activation_entry.pack(fill=tk.X, pady=(0, 10))
        
        # 激活按钮
        button_frame = ttk.Frame(activation_frame)
        button_frame.pack(fill=tk.X, pady=20)
        
        ttk.Button(button_frame, text="激活软件", command=self.validate_activation).pack(side=tk.LEFT, padx=10)
        ttk.Button(button_frame, text="退出", command=self.root.quit).pack(side=tk.RIGHT, padx=10)
        
        # 说明文本
        ttk.Label(activation_frame, text="添加QQ群：816103114，免费获取激活码。", 
                 foreground="blue").pack(pady=10)
        
        # 说明文本
        ttk.Label(activation_frame, text="试用次数已用完，请输入激活码以继续使用软件。", 
                 foreground="red").pack(side=tk.BOTTOM, pady=10)
        
        
    
    def validate_activation(self):
        """验证激活码是否有效"""
        activation_code = self.activation_code_var.get().strip()
        request_code = self.request_code_var.get().strip()
        
        if not activation_code:
            messagebox.showerror("错误", "请输入激活码")
            return
            
        if self.activator.verify_activation(activation_code, request_code):
            if self.save_activation_status(request_code, activation_code):
                messagebox.showinfo("成功", "软件激活成功！")
                self.setup_gui()
            else:
                messagebox.showerror("错误", "激活失败，请重试")
        else:
            messagebox.showerror("错误", "无效的激活码，请检查后重试")

    def setup_gui(self):
        for widget in self.root.winfo_children():
            widget.destroy()
        
        # 创建菜单栏
        self.create_menu()
        
        # 创建主框架
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 创建笔记本控件
        notebook = ttk.Notebook(main_frame)
        notebook.pack(fill=tk.BOTH, expand=True, pady=5)
        
        # 输入参数页
        self.setup_input_tab(notebook)
        
        # 管道元件页
        self.setup_fittings_tab(notebook)
        
        # 结果页
        self.setup_results_tab(notebook)
    
    def create_menu(self):
        """创建菜单栏"""
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # 文件菜单
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="文件", menu=file_menu)
        file_menu.add_command(label="保存数据", command=self.save_data)
        file_menu.add_command(label="读取数据", command=self.load_data)
        file_menu.add_separator()
        file_menu.add_command(label="退出", command=self.root.quit)

        #材料管理菜单
        material_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="材料管理", menu=material_menu)
        material_menu.add_command(label="管道类型管理",command=lambda: self.open_material_dialog('pipe_types'))
        material_menu.add_command(label="管道元件管理",command=lambda: self.open_material_dialog('fittings'))
        material_menu.add_command(label="保温材料管理",command=lambda: self.open_material_dialog('insulation'))
        material_menu.add_command(label="外保护层管理",command=lambda: self.open_material_dialog('protection'))

        # 创建帮助菜单
        about_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="帮助", menu=about_menu)
        about_menu.add_command(label="激活", command=self.show_activation_interface)
        about_menu.add_command(label="下载最新版本", command=self.open_download_url)
        about_menu.add_command(label="关于", command=self.about)
    
    def open_download_url(self):
        webbrowser.open_new("https://gitee.com/yangshu/design-tool/releases")

    def about(self):
        messagebox.showinfo("关于", "管道分析软件\n版本：v0.1\n作者：杨奉全\nQQ群：816103114")

    def save_data(self):
        """保存数据到.pipe文件"""
        try:
            # 收集所有输入数据
            data = {
                'pipe_name': self.pipe_name_var.get(),
                'fluid': self.fluid_var.get(),
                'pipe_type': self.pipe_type_var.get(),
                'inlet_pressure': self.inlet_pressure_var.get(),
                'inlet_temperature': self.inlet_temperature_var.get(),
                'inlet_quality': self.inlet_quality_var.get(),
                'mass_flow': self.mass_flow_var.get(),
                'pipe_length': self.pipe_length_var.get(),
                'pipe_od': self.pipe_od_var.get(),
                'pipe_wall_thickness': self.pipe_wall_thickness_var.get(),
                'segment_length': self.segment_length_var.get(),
                'insulation_material': self.insulation_material_var.get(),
                'insulation_thickness': self.insulation_thickness_var.get(),
                'protection_material': self.protection_material_var.get(),
                'ambient_temperature': self.ambient_temp_var.get(),
                'wind_speed': self.wind_speed_var.get(),
                'fittings_data': self.fittings_data
            }
            
            filename = filedialog.asksaveasfilename(
                defaultextension=".pipe",
                filetypes=[("Pipe文件", "*.pipe"), ("所有文件", "*.*")],
                title="保存数据"
            )
            
            if filename:
                with open(filename, 'w', encoding='utf-8') as f:
                    json.dump(data, f, ensure_ascii=False, indent=2)
                messagebox.showinfo("成功", f"数据已保存到: {filename}")
                
        except Exception as e:
            messagebox.showerror("错误", f"保存数据失败: {e}")
    
    def load_data(self):
        """从.pipe文件读取数据"""
        try:
            filename = filedialog.askopenfilename(
                filetypes=[("Pipe文件", "*.pipe"), ("所有文件", "*.*")],
                title="读取数据"
            )
            
            if filename:
                with open(filename, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                
                # 恢复数据到界面
                self.pipe_name_var.set(data.get('pipe_name', ''))
                self.fluid_var.set(data.get('fluid', ''))
                self.pipe_type_var.set(data.get('pipe_type', ''))
                self.inlet_pressure_var.set(data.get('inlet_pressure', ''))
                self.inlet_temperature_var.set(data.get('inlet_temperature', ''))
                self.inlet_quality_var.set(data.get('inlet_quality', ''))
                self.mass_flow_var.set(data.get('mass_flow', ''))
                self.pipe_length_var.set(data.get('pipe_length', ''))
                self.pipe_od_var.set(data.get('pipe_od', ''))
                self.pipe_wall_thickness_var.set(data.get("pipe_wall_thickness",''))
                self.segment_length_var.set(data.get('segment_length', ''))
                self.insulation_material_var.set(data.get('insulation_material', ''))
                self.insulation_thickness_var.set(data.get('insulation_thickness', ''))
                self.protection_material_var.set(data.get('protection_material', ''))
                self.ambient_temp_var.set(data.get('ambient_temperature', ''))
                self.wind_speed_var.set(data.get('wind_speed', ''))
                
                # 恢复管道元件数据
                self.fittings_data = data.get('fittings_data', [])
                self.load_fittings_to_tree()
                
                messagebox.showinfo("成功", f"数据已从 {filename} 加载")
                
        except Exception as e:
            messagebox.showerror("错误", f"读取数据失败: {e}")
    
    def setup_input_tab(self, notebook):
        """设置输入参数页"""
        self.input_frame = ttk.Frame(notebook, padding="10")
        notebook.add(self.input_frame, text="输入参数")
        
        row = 0
        
        # 工况名称
        ttk.Label(self.input_frame, text="管道名称:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.pipe_name_var = tk.StringVar(value="过热蒸汽管道")
        ttk.Entry(self.input_frame, textvariable=self.pipe_name_var, width=30).grid(row=row, column=1, sticky=tk.W, pady=5, padx=5)
        row += 1
        
        # 流体选择
        # 获取所有可用流体的列表，以逗号分隔的字符串形式返回
        fluids_str = CP.get_global_param_string("fluids_list")

        # 将字符串分割为列表
        fluids_list = fluids_str.split(',')
        ttk.Label(self.input_frame, text="流体:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.fluid_var = tk.StringVar(value="Water")
        self.fluid_combo = ttk.Combobox(self.input_frame, textvariable=self.fluid_var, width=27)
        self.fluid_combo['values'] = fluids_list #('Water', 'Air', 'Ammonia', 'CarbonDioxide', 'R134a')
        self.fluid_combo.grid(row=row, column=1, sticky=tk.W, pady=5, padx=5)
        row += 1
        
        # 管道类型选择
        ttk.Label(self.input_frame, text="管道类型:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.pipe_type_var = tk.StringVar()
        self.pipe_type_combo = ttk.Combobox(self.input_frame, textvariable=self.pipe_type_var, width=27)
        self.pipe_type_combo['values'] = tuple(self.analyzer.material_manager.pipe_types.keys())
        if self.pipe_type_combo['values']:
            self.pipe_type_var.set(self.pipe_type_combo['values'][2])  # 设置默认值
        self.pipe_type_combo.grid(row=row, column=1, sticky=tk.W, pady=5, padx=5)
 
        row += 1
        
        # 入口参数框架
        inlet_frame = ttk.LabelFrame(self.input_frame, text="入口参数", padding="5")
        inlet_frame.grid(row=row, column=0, columnspan=2, sticky=tk.W+tk.E, pady=10)
        row += 1
        
        ttk.Label(inlet_frame, text="压力(MPa, 绝对压力):").grid(row=0, column=0, sticky=tk.W, pady=2)
        self.inlet_pressure_var = tk.StringVar(value="1")
        ttk.Entry(inlet_frame, textvariable=self.inlet_pressure_var, width=15).grid(row=0, column=1, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(inlet_frame, text="温度(°C):").grid(row=0, column=2, sticky=tk.W, pady=2)
        self.inlet_temperature_var = tk.StringVar(value="300")
        ttk.Entry(inlet_frame, textvariable=self.inlet_temperature_var, width=15).grid(row=0, column=3, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(inlet_frame, text="干度:").grid(row=1, column=0, sticky=tk.W, pady=2)
        self.inlet_quality_var = tk.StringVar()
        ttk.Entry(inlet_frame, textvariable=self.inlet_quality_var, width=15).grid(row=1, column=1, sticky=tk.W, pady=2, padx=5)
        ttk.Label(inlet_frame, text="(可选，如指定则忽略温度)").grid(row=1, column=2, columnspan=2, sticky=tk.W, pady=2)
        
        ttk.Label(inlet_frame, text="质量流量(kg/h):").grid(row=2, column=0, sticky=tk.W, pady=2)
        self.mass_flow_var = tk.StringVar(value="10000")
        ttk.Entry(inlet_frame, textvariable=self.mass_flow_var, width=15).grid(row=2, column=1, sticky=tk.W, pady=2, padx=5)
        
        # 管道参数框架
        pipe_frame = ttk.LabelFrame(self.input_frame, text="管道参数", padding="5")
        pipe_frame.grid(row=row, column=0, columnspan=2, sticky=tk.W+tk.E, pady=10)
        row += 1
        
        ttk.Label(pipe_frame, text="管道长度(m):").grid(row=0, column=0, sticky=tk.W, pady=2)
        self.pipe_length_var = tk.StringVar(value="500")
        ttk.Entry(pipe_frame, textvariable=self.pipe_length_var, width=15).grid(row=0, column=1, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(pipe_frame, text="管道外径(mm):").grid(row=0, column=2, sticky=tk.W, pady=2)
        self.pipe_od_var = tk.StringVar(value="168")
        ttk.Entry(pipe_frame, textvariable=self.pipe_od_var, width=15).grid(row=0, column=3, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(pipe_frame, text="分段长度(m):").grid(row=1, column=0, sticky=tk.W, pady=2)
        self.segment_length_var = tk.StringVar(value="50")
        ttk.Entry(pipe_frame, textvariable=self.segment_length_var, width=15).grid(row=1, column=1, sticky=tk.W, pady=2, padx=5)

        ttk.Label(pipe_frame, text="管道壁厚(mm):").grid(row=1, column=2, sticky=tk.W, pady=2)
        self.pipe_wall_thickness_var = tk.StringVar(value="9")
        ttk.Entry(pipe_frame, textvariable=self.pipe_wall_thickness_var, width=15).grid(row=1, column=3, sticky=tk.W, pady=2, padx=5)
        
        # 保温参数框架
        insulation_frame = ttk.LabelFrame(self.input_frame, text="保温参数", padding="5")
        insulation_frame.grid(row=row, column=0, columnspan=2, sticky=tk.W+tk.E, pady=10)
        row += 1
        
        ttk.Label(insulation_frame, text="保温材料:").grid(row=0, column=0, sticky=tk.W, pady=2)
        self.insulation_material_var = tk.StringVar()
        self.insulation_combo = ttk.Combobox(insulation_frame, textvariable=self.insulation_material_var, width=20)
        self.insulation_combo['values'] = tuple(self.analyzer.material_manager.insulation_materials.keys())
        if self.insulation_combo['values']:
            self.insulation_material_var.set(self.insulation_combo['values'][0])
        self.insulation_combo.grid(row=0, column=1, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(insulation_frame, text="保温厚度(mm):").grid(row=0, column=2, sticky=tk.W, pady=2)
        self.insulation_thickness_var = tk.StringVar(value="50")
        ttk.Entry(insulation_frame, textvariable=self.insulation_thickness_var, width=15).grid(row=0, column=3, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(insulation_frame, text="外保护层:").grid(row=1, column=0, sticky=tk.W, pady=2)
        self.protection_material_var = tk.StringVar()
        self.protection_combo = ttk.Combobox(insulation_frame, textvariable=self.protection_material_var, width=20)
        self.protection_combo['values'] = tuple(self.analyzer.material_manager.protection_materials.keys())
        if self.protection_combo['values']:
            self.protection_material_var.set(self.protection_combo['values'][0])
        self.protection_combo.grid(row=1, column=1, sticky=tk.W, pady=2, padx=5)
        
        # 环境参数框架
        env_frame = ttk.LabelFrame(self.input_frame, text="环境参数", padding="5")
        env_frame.grid(row=row, column=0, columnspan=2, sticky=tk.W+tk.E, pady=10)
        row += 1
        
        ttk.Label(env_frame, text="环境温度(°C):").grid(row=0, column=0, sticky=tk.W, pady=2)
        self.ambient_temp_var = tk.StringVar(value="20")
        ttk.Entry(env_frame, textvariable=self.ambient_temp_var, width=15).grid(row=0, column=1, sticky=tk.W, pady=2, padx=5)
        
        ttk.Label(env_frame, text="风速(m/s):").grid(row=0, column=2, sticky=tk.W, pady=2)
        self.wind_speed_var = tk.StringVar(value="3.0")
        ttk.Entry(env_frame, textvariable=self.wind_speed_var, width=15).grid(row=0, column=3, sticky=tk.W, pady=2, padx=5)
        
        # 按钮框架
        button_frame = ttk.Frame(self.input_frame)
        button_frame.grid(row=row, column=0, columnspan=2, pady=20)
        row += 1
        
        ttk.Button(button_frame, text="开始分析", command=self.analyze_pipe).pack(side=tk.LEFT, padx=10)
        ttk.Button(button_frame, text="生成报告", command=self.generate_report).pack(side=tk.LEFT, padx=10)
        ttk.Button(button_frame, text="清空输入", command=self.clear_input).pack(side=tk.LEFT, padx=10)
  
    def setup_fittings_tab(self, notebook):
        """设置管道元件页"""
        fittings_frame = ttk.Frame(notebook, padding="10")
        notebook.add(fittings_frame, text="管道元件")
        
        # 管道元件列表框架
        list_frame = ttk.LabelFrame(fittings_frame, text="管道元件配置", padding="10")
        list_frame.pack(fill=tk.BOTH, expand=True, pady=10)
        
        # 创建树形视图
        columns = ['元件名称', '阻力系数', '数量']
        self.fittings_tree = ttk.Treeview(list_frame, columns=columns, show='headings')
        
        for col in columns:
            self.fittings_tree.heading(col, text=col)
            self.fittings_tree.column(col, width=150)
        
        # 滚动条
        scrollbar = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=self.fittings_tree.yview)
        self.fittings_tree.configure(yscrollcommand=scrollbar.set)
        
        self.fittings_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 添加双击事件
        self.fittings_tree.bind('<Double-1>', self.on_fitting_double_click)
        
        # 添加/删除按钮
        edit_frame = ttk.Frame(list_frame)
        edit_frame.pack(fill=tk.X, pady=5)
        
        ttk.Button(edit_frame, text="添加元件", command=self.add_fitting).pack(side=tk.LEFT, padx=5)
        ttk.Button(edit_frame, text="删除元件", command=self.remove_fitting).pack(side=tk.LEFT, padx=5)
        
        self.fittings_data = []  # 存储管道元件数据
        self.load_fittings_to_tree()
    
    def on_fitting_double_click(self, event):
        """管道元件双击事件"""
        selection = self.fittings_tree.selection()
        if not selection:
            return
        
        item = selection[0]
        index = self.fittings_tree.index(item)
        
        if 0 <= index < len(self.fittings_data):
            fitting_name, current_count = self.fittings_data[index]
            
            # 创建修改对话框
            dialog = tk.Toplevel(self.root)
            dialog.title("修改元件数量")
            dialog.geometry("300x200")
            dialog.resizable(False, False)
            
            ttk.Label(dialog, text=f"修改 {fitting_name} 的数量:").pack(pady=10)
            
            count_var = tk.StringVar(value=str(current_count))
            ttk.Entry(dialog, textvariable=count_var, width=10).pack(pady=5)
            
            def save_count():
                try:
                    new_count = int(count_var.get())
                    if new_count <= 0:
                        raise ValueError("数量必须大于0")
                    
                    self.fittings_data[index] = (fitting_name, new_count)
                    self.load_fittings_to_tree()
                    dialog.destroy()
                except ValueError as e:
                    messagebox.showerror("错误", f"请输入有效的数量: {e}")
            
            ttk.Button(dialog, text="保存", command=save_count).pack(pady=10)
    
    def setup_results_tab(self, notebook):
        """设置结果页"""
        results_frame = ttk.Frame(notebook, padding="10")
        notebook.add(results_frame, text="分析结果")
        
        # 创建笔记本控件用于结果页
        results_notebook = ttk.Notebook(results_frame)
        results_notebook.pack(fill=tk.BOTH, expand=True)
        
        # 文字分析结果页
        text_frame = ttk.Frame(results_notebook)
        results_notebook.add(text_frame, text="文字分析")
        
        # 结果文本控件
        self.results_text = tk.Text(text_frame, wrap=tk.WORD, width=80, height=30)
        results_scrollbar = ttk.Scrollbar(text_frame, orient=tk.VERTICAL, command=self.results_text.yview)
        self.results_text.configure(yscrollcommand=results_scrollbar.set)
        
        self.results_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        results_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 分段数据表格页
        table_frame = ttk.Frame(results_notebook)
        results_notebook.add(table_frame, text="分段数据")
        
        # 创建分段数据表格
        columns = ['段', '距离(m)', '压力(MPa)', '温度(°C)', '干度', '流速(m/s)', '单位长度热损失(W/m)', '单位面积热损失(W/m²)', '表面温度(°C)']
        self.segment_tree = ttk.Treeview(table_frame, columns=columns, show='headings', height=25)
        
        for col in columns:
            self.segment_tree.heading(col, text=col)
            self.segment_tree.column(col, width=100)
        
        # 滚动条
        table_scrollbar = ttk.Scrollbar(table_frame, orient=tk.VERTICAL, command=self.segment_tree.yview)
        self.segment_tree.configure(yscrollcommand=table_scrollbar.set)
        
        self.segment_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        table_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 分析图表页
        chart_frame = ttk.Frame(results_notebook)
        results_notebook.add(chart_frame, text="分析图表")
        
        # 创建带滚动条的图表框架
        chart_container = ttk.Frame(chart_frame)
        chart_container.pack(fill=tk.BOTH, expand=True)
        
        # 创建Canvas和滚动条
        self.chart_canvas = tk.Canvas(chart_container)
        chart_scrollbar = ttk.Scrollbar(chart_container, orient=tk.VERTICAL, command=self.chart_canvas.yview)
        self.chart_canvas.configure(yscrollcommand=chart_scrollbar.set)
        
        # 创建图表框架
        self.chart_frame = ttk.Frame(self.chart_canvas)
        self.chart_canvas_window = self.chart_canvas.create_window((0, 0), window=self.chart_frame, anchor="nw")
        
        # 配置滚动区域
        def configure_scroll_region(event):
            self.chart_canvas.configure(scrollregion=self.chart_canvas.bbox("all"))
        
        self.chart_frame.bind("<Configure>", configure_scroll_region)
        
        self.chart_canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        chart_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 存储分析结果
        self.current_results = None
        self.current_analysis = None
        self.current_fig = None
        self.canvas = None
    
    def open_material_dialog(self, material_type):
        """打开材料管理对话框"""
        dialog = MaterialDialog(self.root, self.analyzer.material_manager, material_type)
        self.refresh_material_combos()  # 确保所有材料类型都能实时更新
    
    def refresh_material_combos(self):
        """刷新材料下拉列表"""
        # 更新保温材料下拉列表
        self.insulation_combo['values'] = tuple(self.analyzer.material_manager.insulation_materials.keys())
        if self.insulation_combo['values'] and not self.insulation_material_var.get():
            self.insulation_material_var.set(self.insulation_combo['values'][0])
        
        # 更新外保护层下拉列表
        self.protection_combo['values'] = tuple(self.analyzer.material_manager.protection_materials.keys())
        if self.protection_combo['values'] and not self.protection_material_var.get():
            self.protection_material_var.set(self.protection_combo['values'][0])
        
        # 更新管道类型下拉列表
        self.pipe_type_combo['values'] = tuple(self.analyzer.material_manager.pipe_types.keys())
        if self.pipe_type_combo['values'] and not self.pipe_type_var.get():
            self.pipe_type_var.set(self.pipe_type_combo['values'][0])
    
    def load_fittings_to_tree(self):
        """加载管道元件到树形视图"""
        # 清空现有数据
        for item in self.fittings_tree.get_children():
            self.fittings_tree.delete(item)
        
        # 添加数据
        for fitting_name, count in self.fittings_data:
            fitting = self.analyzer.material_manager.pipe_fittings.get(fitting_name)
            if fitting:
                self.fittings_tree.insert('', tk.END, values=[fitting_name, f"{fitting.resistance_coef:.3f}", count])
    
    def add_fitting(self):
        """添加管道元件"""
        # 获取所有可用的管道元件
        available_fittings = list(self.analyzer.material_manager.pipe_fittings.keys())
        if not available_fittings:
            messagebox.showwarning("警告", "没有可用的管道元件，请先在材料管理中添加")
            return
        
        # 创建选择对话框
        dialog = tk.Toplevel(self.root)
        dialog.title("添加管道元件")
        dialog.geometry("300x200")
        dialog.resizable(False, False)
        
        ttk.Label(dialog, text="选择管道元件:").pack(pady=10)
        
        fitting_var = tk.StringVar()
        fitting_combo = ttk.Combobox(dialog, textvariable=fitting_var, width=20)
        fitting_combo['values'] = available_fittings
        fitting_combo.set(available_fittings[0])
        fitting_combo.pack(pady=5)
        
        ttk.Label(dialog, text="数量:").pack(pady=5)
        count_var = tk.StringVar(value="1")
        ttk.Entry(dialog, textvariable=count_var, width=10).pack(pady=5)
        
        def save_fitting():
            fitting_name = fitting_var.get()
            try:
                count = int(count_var.get())
                if count <= 0:
                    raise ValueError("数量必须大于0")
                
                self.fittings_data.append((fitting_name, count))
                self.load_fittings_to_tree()
                dialog.destroy()
            except ValueError as e:
                messagebox.showerror("错误", f"请输入有效的数量: {e}")
        
        ttk.Button(dialog, text="添加", command=save_fitting).pack(pady=10)
    
    def remove_fitting(self):
        """删除管道元件"""
        selection = self.fittings_tree.selection()
        if not selection:
            messagebox.showwarning("警告", "请选择要删除的管道元件")
            return
        
        item = selection[0]
        index = self.fittings_tree.index(item)
        
        if 0 <= index < len(self.fittings_data):
            del self.fittings_data[index]
            self.load_fittings_to_tree()
    
    def analyze_pipe(self):
        """分析管道"""
        try:
            # 获取管道类型的粗糙度
            pipe_type_name = self.pipe_type_var.get()
            if pipe_type_name and pipe_type_name in self.analyzer.material_manager.pipe_types:
                pipe_type = self.analyzer.material_manager.pipe_types[pipe_type_name]
                roughness = pipe_type.roughness
            else:
                roughness = 2e-5  # 默认值
            
            # 获取输入参数
            pipe_params = {
                'pipe_name': self.pipe_name_var.get(),
                'fluid': self.fluid_var.get(),
                'pipe_type': self.pipe_type_var.get(),
                'inlet_pressure_pa': float(self.inlet_pressure_var.get()) * 1e6,
                'inlet_temperature_k': float(self.inlet_temperature_var.get()) + 273.15,
                'mass_flow_kg_s': float(self.mass_flow_var.get()) / 3600,
                'pipe_length_m': float(self.pipe_length_var.get()),
                'pipe_od_m': float(self.pipe_od_var.get()) / 1000,
                'pipe_wall_thickness_m': float(self.pipe_wall_thickness_var.get()) / 1000,
                'insulation_thickness_m': float(self.insulation_thickness_var.get()) / 1000,
                'insulation_material': self.insulation_material_var.get(),
                'protection_material': self.protection_material_var.get(),
                # 'roughness_m': roughness,  # 使用管道类型的粗糙度
                'ambient_temperature_k': float(self.ambient_temp_var.get()) + 273.15,
                'wind_speed_m_s': float(self.wind_speed_var.get()),
                'segment_length_m': float(self.segment_length_var.get()),
                'fittings_data': self.fittings_data
            }

            # 如果指定了干度，则使用干度
            quality_text = self.inlet_quality_var.get()
            if quality_text:
                pipe_params['inlet_quality'] = float(quality_text)
            
            # 执行分析
            df_results, analysis_results, fig = self.analyzer.analyze(pipe_params)

            # 存储结果
            self.current_results = df_results
            self.current_analysis = analysis_results
            self.current_fig = fig
            
            # 显示结果
            self.display_results(analysis_results)
            
            # 显示分段数据表格
            self.display_segment_table(df_results)
            
            # 显示图表
            self.display_chart(fig)
            
            messagebox.showinfo("成功", "管道分析完成！")
            
        except ValueError as e:
            messagebox.showerror("错误", f"输入参数错误: {e}")
        except Exception as e:
            messagebox.showerror("错误", f"分析过程中发生错误: {e}")
    
    def display_results(self, analysis_results):
        """显示分析结果"""
        self.results_text.delete(1.0, tk.END)

        # 基本结果
        self.results_text.insert(tk.END, "管道分析结果\n", "title")
        self.results_text.insert(tk.END, "="*50 + "\n\n")
        
        self.results_text.insert(tk.END, "入口参数:\n", "subtitle")
        self.results_text.insert(tk.END, f"  压力: {analysis_results['inlet_pressure_pa'] / 1e6:.6f} MPa\n")
        self.results_text.insert(tk.END, f"  温度: {(analysis_results['inlet_temperature_k'] - 273.15):.2f} °C\n")
        self.results_text.insert(tk.END, f"  干度: {analysis_results['inlet_quality']:.4f}\n")
        self.results_text.insert(tk.END, f"  流速: {analysis_results['inlet_velocity_m_s']:.2f} m/s\n\n")
        
        self.results_text.insert(tk.END, "出口参数:\n", "subtitle")
        self.results_text.insert(tk.END, f"  压力: {analysis_results['outlet_pressure_pa'] / 1e6:.6f} MPa\n")
        self.results_text.insert(tk.END, f"  温度: {analysis_results['outlet_temperature_k'] - 273.15:.2f} °C\n")
        self.results_text.insert(tk.END, f"  干度: {analysis_results['outlet_quality']:.4f}\n")
        self.results_text.insert(tk.END, f"  流速: {analysis_results['outlet_velocity_m_s']:.2f} m/s\n\n")
        
        self.results_text.insert(tk.END, "压力损失分析:\n", "subtitle")
        self.results_text.insert(tk.END, f"  总压力降: {analysis_results['total_pressure_drop_pa'] / 1e6:.4f} MPa\n")
        self.results_text.insert(tk.END, f"  单位压降: {analysis_results['pressure_drop_pa_m']:.1f} kPa/km\n")
        self.results_text.insert(tk.END, f"  压降比例: {analysis_results['pressure_ratio']:.3%}\n")
        self.results_text.insert(tk.END, f"  沿程阻力: {analysis_results['total_friction_drop_pa'] / 1e6:.6f} MPa\n")
        self.results_text.insert(tk.END, f"  局部阻力: {analysis_results['total_fittings_drop_pa'] / 1e6:.6f} MPa\n")
        self.results_text.insert(tk.END, f"  管道粗糙度: {analysis_results['pipe_roughness_m']:.6f} m\n\n")
        
        self.results_text.insert(tk.END, "热损失分析:\n", "subtitle")
        self.results_text.insert(tk.END, f"  总热损失: {analysis_results['total_heat_loss_w'] / 1000:.1f} kW\n")
        self.results_text.insert(tk.END, f"  平均单位热损失: {analysis_results['avg_heat_loss_per_m_w']:.1f} W/m\n")
        self.results_text.insert(tk.END, f"  平均单位面积热损失: {analysis_results['avg_heat_loss_per_area_w']:.1f} W/m²\n")
        self.results_text.insert(tk.END, f"  最大表面温度: {analysis_results['max_surface_temp_k'] - 273.15:.2f} °C\n")
        self.results_text.insert(tk.END, f"  平均表面温度: {analysis_results['avg_surface_temp_k'] - 273.15:.2f} °C\n\n")
        
        self.results_text.insert(tk.END, "流动特性:\n", "subtitle")
        self.results_text.insert(tk.END, f"  平均流速: {analysis_results['avg_velocity_m_s']:.2f} m/s\n")
        self.results_text.insert(tk.END, f"  最大流速: {analysis_results['max_velocity_m_s']:.2f} m/s\n")
        self.results_text.insert(tk.END, f"  最小流速: {analysis_results['min_velocity_m_s']:.2f} m/s\n")
        self.results_text.insert(tk.END, f"  平均雷诺数: {analysis_results['avg_reynolds']:.0f}\n")
        self.results_text.insert(tk.END, f"  平均摩擦系数: {analysis_results['avg_friction']:.5f}\n")
        
        # 配置样式
        self.results_text.tag_configure("title", font=("Arial", 12, "bold"))
        self.results_text.tag_configure("subtitle", font=("Arial", 10, "bold"))
    
    def display_segment_table(self, df_results):
        """显示分段数据表格"""
        # 清空现有数据
        for item in self.segment_tree.get_children():
            self.segment_tree.delete(item)
        
        # 添加数据
        for _, row in df_results.iterrows():
            self.segment_tree.insert('', tk.END, values=[
                int(row['segment']),
                f"{row['distance_m']:.0f}",
                f"{row['pressure_pa'] / 1e6:.6f}",
                f"{row['temperature_k'] - 273.15:.2f}",
                f"{row['quality']:.4f}",
                f"{row['velocity_m_s']:.2f}",
                f"{row['heat_loss_per_m_w']:.1f}",
                f"{row['heat_loss_per_area_w']:.1f}",
                f"{row['surface_temp_k'] - 273.15:.1f}"
            ])
    
    def display_chart(self, fig):
        """在GUI中显示图表"""
        # 清除现有图表
        for widget in self.chart_frame.winfo_children():
            widget.destroy()
        
        if fig:
            # 将图表嵌入到GUI中
            self.canvas = FigureCanvasTkAgg(fig, self.chart_frame)
            self.canvas.draw()
            self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
    
    def generate_report(self):
        """生成Word报告"""
        if self.current_results is None or self.current_analysis is None:
            messagebox.showwarning("警告", "请先进行管道分析")
            return
        
        # 选择保存路径
        filename = filedialog.asksaveasfilename(
            defaultextension=".docx",
            filetypes=[("Word文档", "*.docx"), ("所有文件", "*.*")],
            title="保存分析报告"
        )
        
        if filename:
            try:
                # 获取管道类型的粗糙度
                pipe_type_name = self.pipe_type_var.get()
                if pipe_type_name and pipe_type_name in self.analyzer.material_manager.pipe_types:
                    pipe_type = self.analyzer.material_manager.pipe_types[pipe_type_name]
                    roughness = pipe_type.roughness
                else:
                    roughness = 2e-5  # 默认值
                
                # 获取输入参数
                pipe_params = {
                'pipe_name': self.pipe_name_var.get(),
                'fluid': self.fluid_var.get(),
                'inlet_pressure_pa': float(self.inlet_pressure_var.get()) * 1e6,
                'inlet_temperature_k': float(self.inlet_temperature_var.get()) + 273.15,
                'mass_flow_kg_s': float(self.mass_flow_var.get()) / 3600,
                'pipe_length_m': float(self.pipe_length_var.get()),
                'pipe_od_m': float(self.pipe_od_var.get()) / 1000,
                'pipe_wall_thickness_m': float(self.pipe_wall_thickness_var.get()) / 1000,
                'insulation_thickness_m': float(self.insulation_thickness_var.get()) / 1000,
                'insulation_material': self.insulation_material_var.get(),
                'protection_material': self.protection_material_var.get(),
                'roughness_m': roughness,  # 使用管道类型的粗糙度
                'ambient_temperature_k': float(self.ambient_temp_var.get()) + 273.15,
                'wind_speed_m_s': float(self.wind_speed_var.get()),
                'segment_length_m': float(self.segment_length_var.get()),
                'fittings_data': self.fittings_data
                }
                
                # 生成报告
                success = self.analyzer.generate_word_report(
                    pipe_params, 
                    self.current_results, 
                    self.current_analysis, 
                    self.current_fig,
                    filename
                )
                
                if success:
                    messagebox.showinfo("成功", f"报告已保存至: {filename}")
                else:
                    messagebox.showerror("错误", "生成报告失败")
                    
            except Exception as e:
                messagebox.showerror("错误", f"生成报告时发生错误: {e}")
    
    def clear_input(self):
        """清空输入"""
        self.pipe_name_var.set("")
        self.inlet_pressure_var.set("")
        self.inlet_temperature_var.set("")
        self.inlet_quality_var.set("")
        self.mass_flow_var.set("")
        self.pipe_length_var.set("")
        self.pipe_od_var.set("")
        self.pipe_wall_thickness_var.set("")
        self.insulation_thickness_var.set("")
        self.ambient_temp_var.set("")
        self.wind_speed_var.set("")
        self.segment_length_var.set("")
        
        # 清空管道元件
        self.fittings_data = []
        self.load_fittings_to_tree()
        
        # 清空结果
        self.results_text.delete(1.0, tk.END)
        self.current_results = None
        self.current_analysis = None
        if self.current_fig:
            plt.close(self.current_fig)
            self.current_fig = None
        
        # 清除图表
        for widget in self.chart_frame.winfo_children():
            widget.destroy()
        
        # 清除分段数据表格
        for item in self.segment_tree.get_children():
            self.segment_tree.delete(item)