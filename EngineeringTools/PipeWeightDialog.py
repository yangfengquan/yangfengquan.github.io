import tkinter as tk
from tkinter import ttk, messagebox
import math

class PipeWeightDialog:
    """管子重量计算对话框（改进版）
    
    公式：重量(kg) = π × (外径² - (外径-2×壁厚)²) / 4 × 长度 × 密度 / 1000
    其中：
    - 外径、壁厚单位为毫米(mm)
    - 长度单位为米(m)
    - 密度单位为g/cm³
    """
    def __init__(self, parent):
        # 创建顶层窗口
        self.dialog = tk.Toplevel(parent)
        self.dialog.title("管子重量计算器")
        self.dialog.geometry("420x350")
        self.dialog.resizable(False, False)
        
        # 存储计算结果
        self.result = None
        
        # 创建输入变量
        self.outer_diameter = tk.StringVar()  # 外径(mm)
        self.wall_thickness = tk.StringVar()  # 壁厚(mm)
        self.length = tk.StringVar()          # 长度(m)
        self.density = tk.StringVar(value="7.85")  # 密度(g/cm³)，默认钢的密度
        
        # 设置UI
        self._setup_ui()
        
        # 模态设置
        self.dialog.grab_set()
        parent.wait_window(self.dialog)
    
    def _setup_ui(self):
        """设置用户界面"""
        # 创建主框架
        main_frame = ttk.Frame(self.dialog, padding=20)
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 标题标签
        title_label = ttk.Label(
            main_frame, 
            text="请输入管子参数", 
            font=("SimHei", 14, "bold"),
            foreground="#333333"
        )
        title_label.grid(row=0, column=0, columnspan=2, pady=(0, 20))
        
        # 外径输入
        ttk.Label(main_frame, text="外径(mm)：", font=("SimHei", 10)).grid(
            row=1, column=0, sticky=tk.W, pady=8)
        ttk.Entry(
            main_frame, 
            textvariable=self.outer_diameter, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=1, column=1, pady=8)
        
        # 壁厚输入
        ttk.Label(main_frame, text="壁厚(mm)：", font=("SimHei", 10)).grid(
            row=2, column=0, sticky=tk.W, pady=8)
        ttk.Entry(
            main_frame, 
            textvariable=self.wall_thickness, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=2, column=1, pady=8)
        
        # 长度输入
        ttk.Label(main_frame, text="长度(m)：", font=("SimHei", 10)).grid(
            row=3, column=0, sticky=tk.W, pady=8)
        ttk.Entry(
            main_frame, 
            textvariable=self.length, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=3, column=1, pady=8)
        
        # 密度输入
        ttk.Label(main_frame, text="密度(g/cm³)：", font=("SimHei", 10)).grid(
            row=4, column=0, sticky=tk.W, pady=8)
        density_entry = ttk.Entry(
            main_frame, 
            textvariable=self.density, 
            width=20,
            font=("SimHei", 10)
        )
        density_entry.grid(row=4, column=1, pady=8)
        # 添加提示标签
        ttk.Label(
            main_frame, 
            text="(钢默认7.85，不锈钢7.93)", 
            font=("SimHei", 8),
            foreground="#666666"
        ).grid(row=5, column=1, sticky=tk.W)
        
        # 结果显示
        self.result_label = ttk.Label(
            main_frame, 
            text="重量计算结果将显示在这里", 
            font=("SimHei", 10),
            foreground="#666666",
            justify=tk.CENTER
        )
        self.result_label.grid(row=6, column=0, columnspan=2, pady=15)
        
        # 按钮区域
        btn_frame = ttk.Frame(main_frame)
        btn_frame.grid(row=7, column=0, columnspan=2, pady=10)
        
        # 计算按钮
        calc_btn = ttk.Button(
            btn_frame, 
            text="计算重量", 
            command=self.calculate_weight
        )
        calc_btn.pack(side=tk.LEFT, padx=10)
        
        # 确认按钮
        confirm_btn = ttk.Button(
            btn_frame, 
            text="确认", 
            command=self._on_confirm
        )
        confirm_btn.pack(side=tk.LEFT, padx=10)
        
        # 取消按钮
        cancel_btn = ttk.Button(
            btn_frame, 
            text="取消", 
            command=self._on_cancel
        )
        cancel_btn.pack(side=tk.LEFT, padx=10)
    
    def calculate_weight(self):
        """计算管子重量"""
        try:
            # 获取输入值并转换为浮点数
            outer = float(self.outer_diameter.get())
            thickness = float(self.wall_thickness.get())
            length = float(self.length.get())
            density = float(self.density.get())
            
            # 验证输入值是否合理
            if outer <= 0 or thickness <= 0 or length <= 0 or density <= 0:
                messagebox.showerror("输入错误", "所有参数必须大于0")
                return
                
            if outer <= 2 * thickness:
                messagebox.showerror("输入错误", "外径必须大于两倍壁厚")
                return
            
            # 计算内径：外径 - 2×壁厚
            inner = outer - 2 * thickness
            
            # 计算重量 (kg)
            # 公式：重量 = π × (外径² - 内径²) / 4 × 长度 × 密度 / 1000
            weight = math.pi * (outer**2 - inner**2) / 4 * length * density / 1000
            
            # 保留两位小数
            weight = round(weight, 2)
            
            # 显示结果
            self.result = weight
            self.result_label.config(
                text=f"管子重量：{weight} kg",
                foreground="#0066cc",
                font=("SimHei", 12, "bold")
            )
            
        except ValueError:
            messagebox.showerror("输入错误", "请输入有效的数字")
        except Exception as e:
            messagebox.showerror("错误", f"计算出错：{str(e)}")
    
    def _on_confirm(self):
        """确认按钮回调"""
        if self.result is None:
            messagebox.showinfo("提示", "请先计算重量")
            return
        self.dialog.destroy()
    
    def _on_cancel(self):
        """取消按钮回调"""
        self.result = None
        self.dialog.destroy()
    