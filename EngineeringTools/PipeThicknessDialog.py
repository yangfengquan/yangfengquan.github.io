import tkinter as tk
from tkinter import ttk, messagebox
import math

class PipeThicknessDialog:
    def __init__(self, parent):
        self.dialog = tk.Toplevel(parent)
        self.dialog.title("GB/T20801.3-2020 管道壁厚计算")
        self.dialog.geometry("500x620")
        self.dialog.resizable(False, False)
        self.dialog.transient(parent)  # 设置为主窗口的临时窗口
        self.dialog.grab_set()  # 模态对话框，阻止对主窗口的操作
        
        # 居中显示对话框
        self.center_dialog(parent)
        
        self.create_widgets()
        self.set_default_values()
    
    def center_dialog(self, parent):
        """使对话框在父窗口中央显示"""
        self.dialog.update_idletasks()
        width = self.dialog.winfo_width()
        height = self.dialog.winfo_height()
        x = parent.winfo_x() + (parent.winfo_width() // 2) - (width // 2)
        y = parent.winfo_y() + (parent.winfo_height() // 2) - (height // 2)
        self.dialog.geometry(f"+{x}+{y}")
    
    def create_widgets(self):
        # 创建主框架
        main_frame = ttk.Frame(self.dialog, padding="15")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 标题
        title_label = ttk.Label(main_frame, text="GB/T20801.3-2020 管道壁厚计算", 
                               font=("Arial", 14, "bold"))
        title_label.grid(row=0, column=0, columnspan=2, pady=(0, 15))
        
        # 设计参数输入区域
        params_frame = ttk.LabelFrame(main_frame, text="设计参数", padding="10")
        params_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # 设计压力
        ttk.Label(params_frame, text="设计压力 P (MPa):").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.pressure_var = tk.DoubleVar()
        pressure_entry = ttk.Entry(params_frame, textvariable=self.pressure_var, width=12)
        pressure_entry.grid(row=0, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 管道外径
        ttk.Label(params_frame, text="管道外径 D (mm):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.diameter_var = tk.DoubleVar()
        diameter_entry = ttk.Entry(params_frame, textvariable=self.diameter_var, width=12)
        diameter_entry.grid(row=1, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 许用应力
        ttk.Label(params_frame, text="许用应力 [σ]t (MPa):").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.stress_var = tk.DoubleVar()
        stress_entry = ttk.Entry(params_frame, textvariable=self.stress_var, width=12)
        stress_entry.grid(row=2, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        ttk.Label(params_frame, text="(请查阅GB/T20801.2-2020表A.1)").grid(row=2, column=2, sticky=tk.W, pady=5, padx=(5, 0))
        
        # 焊接接头系数
        ttk.Label(params_frame, text="焊接接头系数 φ:").grid(row=3, column=0, sticky=tk.W, pady=5)
        self.joint_var = tk.DoubleVar(value=1.0)
        joint_combo = ttk.Combobox(params_frame, textvariable=self.joint_var, 
                                  values=[0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], 
                                  state="readonly", width=10)
        joint_combo.grid(row=3, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 附加厚度参数区域
        thickness_frame = ttk.LabelFrame(main_frame, text="附加厚度参数", padding="10")
        thickness_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # 腐蚀裕量
        ttk.Label(thickness_frame, text="腐蚀裕量 C1 (mm):").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.corrosion_var = tk.DoubleVar(value=1.5)  # 默认值1.5mm
        corrosion_entry = ttk.Entry(thickness_frame, textvariable=self.corrosion_var, width=12)
        corrosion_entry.grid(row=0, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 机械加工深度
        ttk.Label(thickness_frame, text="机械加工深度 C2 (mm):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.machining_var = tk.DoubleVar(value=0.0)  # 默认值0mm
        machining_entry = ttk.Entry(thickness_frame, textvariable=self.machining_var, width=12)
        machining_entry.grid(row=1, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 材料负偏差
        ttk.Label(thickness_frame, text="材料负偏差 (%):").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.tolerance_var = tk.DoubleVar(value=12.5)  # 默认值12.5%
        tolerance_entry = ttk.Entry(thickness_frame, textvariable=self.tolerance_var, width=12)
        tolerance_entry.grid(row=2, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 计算按钮
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=3, column=0, columnspan=2, pady=15)
        
        ttk.Button(button_frame, text="计算壁厚", command=self.calculate_thickness, width=12).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="重置参数", command=self.set_default_values, width=12).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="关闭", command=self.dialog.destroy, width=12).pack(side=tk.LEFT, padx=5)
        
        # 结果显示区域
        result_frame = ttk.LabelFrame(main_frame, text="计算结果", padding="10")
        result_frame.grid(row=4, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # 计算壁厚
        ttk.Label(result_frame, text="计算壁厚 δ (mm):").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.calc_thickness_var = tk.StringVar(value="")
        ttk.Label(result_frame, textvariable=self.calc_thickness_var, 
                 font=("Arial", 10, "bold"), foreground="blue").grid(row=0, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 包含腐蚀裕量的壁厚
        ttk.Label(result_frame, text="包含腐蚀裕量的壁厚 δd (mm):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.design_thickness_var = tk.StringVar(value="")
        ttk.Label(result_frame, textvariable=self.design_thickness_var, 
                 font=("Arial", 10, "bold"), foreground="blue").grid(row=1, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 考虑材料厚度负偏差的壁厚
        ttk.Label(result_frame, text="考虑负偏差的壁厚 δc (mm):").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.tolerance_thickness_var = tk.StringVar(value="")
        ttk.Label(result_frame, textvariable=self.tolerance_thickness_var, 
                 font=("Arial", 10, "bold"), foreground="blue").grid(row=2, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 圆整名义厚度
        ttk.Label(result_frame, text="圆整名义厚度 δn (mm):").grid(row=3, column=0, sticky=tk.W, pady=5)
        self.nominal_thickness_var = tk.StringVar(value="")
        ttk.Label(result_frame, textvariable=self.nominal_thickness_var, 
                 font=("Arial", 10, "bold"), foreground="red").grid(row=3, column=1, sticky=tk.W, pady=5, padx=(10, 0))
        
        # 绑定回车键到计算按钮
        self.dialog.bind('<Return>', lambda event: self.calculate_thickness())
        
        # 绑定ESC键到关闭按钮
        self.dialog.bind('<Escape>', lambda event: self.dialog.destroy())
    
    def set_default_values(self):
        """设置默认值"""
        self.pressure_var.set(1.0)
        self.diameter_var.set(100.0)
        self.stress_var.set(130.0)
        self.joint_var.set(1.0)
        self.corrosion_var.set(1.5)  # 默认腐蚀裕量1.5mm
        self.machining_var.set(0.0)  # 默认机械加工深度0mm
        self.tolerance_var.set(12.5)  # 默认材料负偏差12.5%
        
        self.calc_thickness_var.set("")
        self.design_thickness_var.set("")
        self.tolerance_thickness_var.set("")
        self.nominal_thickness_var.set("")
    
    def calculate_thickness(self):
        """计算管道壁厚"""
        try:
            # 获取输入值
            P = self.pressure_var.get()  # 设计压力, MPa
            D = self.diameter_var.get()  # 管道外径, mm
            σt = self.stress_var.get()  # 许用应力, MPa
            φ = self.joint_var.get()  # 焊接接头系数
            C1 = self.corrosion_var.get()  # 腐蚀裕量, mm
            C2 = self.machining_var.get()  # 机械加工深度, mm
            tolerance = self.tolerance_var.get()  # 材料负偏差, %
            
            # 验证输入
            if P <= 0:
                messagebox.showerror("输入错误", "设计压力必须大于0")
                return
            
            if D <= 0:
                messagebox.showerror("输入错误", "管道外径必须大于0")
                return
            
            if σt <= 0:
                messagebox.showerror("输入错误", "许用应力必须大于0")
                return
            
            if tolerance < 0 or tolerance >= 100:
                messagebox.showerror("输入错误", "材料负偏差必须是0-100之间的百分比值")
                return
            
            # 计算壁厚 δ = P * D / (2 * [σ]t * φ + P)
            δ = (P * D) / (2 * σt * φ + P)
            
            # 包含腐蚀裕量的壁厚 δd = δ + C1 + C2
            δd = δ + C1 + C2
            
            # 考虑材料厚度负偏差的壁厚 δc = δd / (1 - 材料负偏差/100)
            δc = δd / (1 - tolerance/100)
            
            # 圆整到标准壁厚系列
            δn = self.round_to_standard_thickness(δc)
            
            # 显示结果
            self.calc_thickness_var.set(f"{δ:.4f} mm")
            self.design_thickness_var.set(f"{δd:.4f} mm")
            self.tolerance_thickness_var.set(f"{δc:.4f} mm")
            self.nominal_thickness_var.set(f"{δn} mm")
            
        except ValueError as e:
            messagebox.showerror("输入错误", "请输入有效的数值")
        except ZeroDivisionError as e:
            messagebox.showerror("计算错误", "计算过程中出现除零错误")
        except Exception as e:
            messagebox.showerror("错误", f"计算过程中出现错误: {str(e)}")
    
    def round_to_standard_thickness(self, thickness):
        """将计算厚度圆整到标准壁厚系列"""
        # 标准壁厚系列 (GB/T 17395)
        standard_series = [
            1.0, 1.2, 1.4, 1.5, 1.6, 1.8, 2.0, 2.2, 2.5, 2.8, 3.0, 3.2, 3.5, 3.8, 4.0,
            4.2, 4.5, 4.8, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0,
            11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
            22.0, 24.0, 25.0, 26.0, 28.0, 30.0, 32.0, 34.0, 35.0, 36.0, 38.0, 40.0,
            42.0, 45.0, 48.0, 50.0, 52.0, 55.0, 58.0, 60.0, 65.0, 70.0, 75.0, 80.0,
            85.0, 90.0, 95.0, 100.0
        ]
        
        # 找到大于等于计算值的最小标准壁厚
        for std_thickness in standard_series:
            if std_thickness >= thickness:
                return std_thickness
        
        # 如果计算值大于最大标准壁厚，返回计算值（向上取整）
        return math.ceil(thickness)
    
    def show(self):
        """显示对话框并等待"""
        self.dialog.wait_window()


# 示例主应用程序，展示如何使用这个对话框
# class MainApplication:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("管道设计软件")
#         self.root.geometry("400x200")
#         self.root.resizable(False, False)
        
#         # 居中显示主窗口
#         self.center_window()
        
#         self.create_widgets()
    
#     def center_window(self):
#         """使窗口在屏幕中央显示"""
#         self.root.update_idletasks()
#         width = self.root.winfo_width()
#         height = self.root.winfo_height()
#         x = (self.root.winfo_screenwidth() // 2) - (width // 2)
#         y = (self.root.winfo_screenheight() // 2) - (height // 2)
#         self.root.geometry(f"+{x}+{y}")
    
#     def create_widgets(self):
#         # 主界面
#         main_frame = ttk.Frame(self.root, padding="20")
#         main_frame.pack(fill=tk.BOTH, expand=True)
        
#         # 标题
#         title_label = ttk.Label(main_frame, text="管道设计软件", 
#                                font=("Arial", 16, "bold"))
#         title_label.pack(pady=20)
        
#         # 功能按钮
#         ttk.Button(main_frame, text="管道壁厚计算", 
#                   command=self.open_thickness_calculator, width=20).pack(pady=10)
        
#         ttk.Button(main_frame, text="其他功能", 
#                   command=self.other_function, width=20).pack(pady=10)
        
#         ttk.Button(main_frame, text="退出", 
#                   command=self.root.quit, width=20).pack(pady=10)
    
#     def open_thickness_calculator(self):
#         """打开管道壁厚计算对话框"""
#         dialog = PipeThicknessDialog(self.root)
#         dialog.show()
    
#     def other_function(self):
#         """其他功能（示例）"""
#         messagebox.showinfo("提示", "这是其他功能的示例")


# def main():
#     root = tk.Tk()
#     app = MainApplication(root)
#     root.mainloop()


# if __name__ == "__main__":
#     main()