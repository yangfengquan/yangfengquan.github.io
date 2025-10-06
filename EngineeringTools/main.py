import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import webbrowser
from PropDialog import PropDialog
from PipeWeightDialog import PipeWeightDialog
from PipeFlowDialog import PipeFlowDialog
from PipeThicknessDialog import PipeThicknessDialog
from PipeLoadDialog import PipeLoadDialog
from PipeResistanceDialog import PipeResistanceDialog
from SafetyValveDialog import SafetyValveDialog
from PumpPowerDialog import PumpCalculatorDialog
from SteelPipeSizeSeries import PipeSizeSeriesDialog
from Activation import RegistryStorage, HardwareActivation
class App(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.root = root
        self.root.title("工程设计工具集")
        #self.root.geometry("1200x1000")
        self.root.state("zoomed")

        # 初始化注册表存储
        self.registry_storage = RegistryStorage("YTools")
        
        # 初始化激活器
        self.activator = HardwareActivation(secret_key="YTools2025")
        
        # 检查软件状态
        self.check_software_status()

        #self.setup_gui()

    def check_software_status(self):
        """检查软件是已激活、试用中还是已过期"""
        # 检查是否已激活
        if self.is_activated():
            self._setup_ui()
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
            self._setup_ui()
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
                self._setup_ui()
            else:
                messagebox.showerror("错误", "激活失败，请重试")
        else:
            messagebox.showerror("错误", "无效的激活码，请检查后重试")

    def _setup_menu(self):
        # 创建主菜单栏
        menubar = tk.Menu()

        # 创建文件菜单
        file_menu = tk.Menu(menubar, tearoff=0)  # tearoff=0 防止菜单被撕下
        file_menu.add_separator()  # 添加分隔线
        file_menu.add_command(label="退出", command=self.root.quit, accelerator="Ctrl+Q")

        material_menu = tk.Menu(menubar, tearoff=0)
        material_menu.add_command(label="物质物性", command=self.open_propDialog)
        

        pipe_menu = tk.Menu(menubar, tearoff=0)
        pipe_menu.add_command(label="钢管尺寸系列", command=self.open_pipeSizeSeriesDialog)
        pipe_menu.add_command(label="钢管重量", command=self.open_pipeWeightDialog)
        pipe_menu.add_command(label="管道荷载", command=self.open_pipeLoadDialog)
        pipe_menu.add_command(label="管径计算", command=self.open_pipeFlowDialog)
        pipe_menu.add_command(label="管道阻力", command=self.open_pipeResistanceDialog)
        pipe_menu.add_command(label="管子壁厚", command=self.open_pipeThicknessDialog)

        equipment_menu = tk.Menu(menubar, tearoff=0)
        equipment_menu.add_command(label="安全阀泄放面积", command=self.open_safetyValveDialog)
        equipment_menu.add_command(label="泵轴功率", command=self.open_pumpPowerDialog)

        # 创建帮助菜单
        help_menu = tk.Menu(menubar, tearoff=0)
        help_menu.add_command(label="帮助", command=self.app_help)
        help_menu.add_command(label="下载最新版本", command=self.open_download_url)
        help_menu.add_command(label="激活", command=self.show_activation_interface)
        help_menu.add_command(label="关于", command=self.about)

        # 将子菜单添加到主菜单栏
        menubar.add_cascade(label="文件", menu=file_menu)
        menubar.add_cascade(label="物质", menu=material_menu)
        menubar.add_cascade(label="管道", menu=pipe_menu)
        menubar.add_cascade(label="设备", menu=equipment_menu)
        menubar.add_cascade(label="帮助", menu=help_menu)

        # 将菜单栏设置到窗口
        self.root.config(menu=menubar)
    
    def _setup_ui(self):
        for widget in self.root.winfo_children():
            widget.destroy()
        
        self._setup_menu()

        # 多行文本内容
        multiline_text = """作者:杨奉全
QQ群：816103114"""
        ttk.Label(
            root,
            text=multiline_text,
            font=("SimHei", 24),  # 使用黑体，字号24
            foreground="#808080",  # 灰色
            anchor="center",       # 内容整体居中
            justify="center",      # 多行文本时每行居中对齐
            wraplength=500,        # 文本宽度超过500像素时自动换行
        ).pack(expand=True, fill="both")

    def new_file(self):
        print("新建文件")

    def open_file(self):
        print("打开文件")

    def open_download_url(self):
        webbrowser.open_new("https://gitee.com/yangshu/design-tool/releases")

    def app_help(self):
        messagebox.showinfo("帮助", "点击菜单栏，进入相应功能模块。")

    def about(self):
        messagebox.showinfo("关于", "工程设计工具集\n版本：v0.2\n作者：杨奉全\nQQ群：816103114")
    
    def open_propDialog(self):
        PropDialog(parent=self.root)
    
    def open_pipeFlowDialog(self):
        PipeFlowDialog(parent=self.root)

    def open_pipeResistanceDialog(self):
        PipeResistanceDialog(parent=self.root)
        
    def open_pipeWeightDialog(self):
        PipeWeightDialog(parent=self.root)

    def open_pipeThicknessDialog(self):
        PipeThicknessDialog(parent=self.root)
        
    def open_pipeLoadDialog(self):
        PipeLoadDialog(parent=self.root)
    
    def open_safetyValveDialog(self):
        SafetyValveDialog(parent=self.root)

    def open_pumpPowerDialog(self):
        dlg = PumpCalculatorDialog(parent=self.root)
        dlg.grab_set()

    def open_pipeSizeSeriesDialog(self):
        PipeSizeSeriesDialog(parent=self.root)

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()