import tkinter as tk
from tkinter import ttk, messagebox
import math

class PipeLoadDialog(tk.Toplevel):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.parent = parent
        self.title("管道重量及支撑跨距计算")
        self.geometry("700x550")
        self.configure(bg="#f0f0f0")
        
        # 外保护层材料及其密度映射表 (kg/m³)
        self.cover_materials = {
            "钢板": 7850,
            "铝皮": 2700,
            "镀锌铁皮": 7800,
            "PVC": 1400,
            "玻璃布": 250,
            "其他": 0  # 用于自定义密度
        }
        
        # 设置中文字体支持
        self.style = ttk.Style()
        self.style.configure(".", font=("SimHei", 10))
        
        # 如果有父窗口，设置为模态对话框
        if parent:
            self.transient(parent)
            self.grab_set()
        
        self.create_widgets()
        
    def create_widgets(self):
        # 创建标签页
        tab_control = ttk.Notebook(self)
        
        # 管道参数标签页
        self.tab_params = ttk.Frame(tab_control)
        tab_control.add(self.tab_params, text="管道参数")
        
        # 计算结果标签页
        self.tab_results = ttk.Frame(tab_control)
        tab_control.add(self.tab_results, text="计算结果")
        
        tab_control.pack(expand=1, fill="both", padx=10, pady=10)
        
        # 填充管道参数标签页
        self.create_params_tab()
        
        # 填充计算结果标签页
        self.create_results_tab()
        
        # 创建按钮框架，使按钮在同一行显示
        button_frame = ttk.Frame(self)
        button_frame.pack(pady=10)
        
        # 计算按钮
        btn_calculate = ttk.Button(button_frame, text="计算", command=self.calculate)
        btn_calculate.pack(side=tk.LEFT, padx=10, ipadx=20, ipady=5)
        
        # 重置按钮
        btn_reset = ttk.Button(button_frame, text="重置", command=self.reset)
        btn_reset.pack(side=tk.LEFT, padx=10, ipadx=20, ipady=5)
        
        # 关闭按钮
        btn_close = ttk.Button(button_frame, text="关闭", command=self.destroy)
        btn_close.pack(side=tk.LEFT, padx=10, ipadx=20, ipady=5)
    
    def create_params_tab(self):
        # 创建一个框架来容纳所有输入字段
        frame = ttk.LabelFrame(self.tab_params, text="输入参数")
        frame.pack(fill="both", expand=True, padx=10, pady=10)
        
        # 使用网格布局排列输入字段
        row = 0
        
        # 管道尺寸参数
        ttk.Label(frame, text="管道外径 (mm):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.outer_diameter = ttk.Entry(frame, width=15)
        self.outer_diameter.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.outer_diameter.insert(0, "108")
        
        row += 1
        ttk.Label(frame, text="管道壁厚 (mm):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.wall_thickness = ttk.Entry(frame, width=15)
        self.wall_thickness.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.wall_thickness.insert(0, "4")
        
        row += 1
        ttk.Label(frame, text="管道材质密度 (kg/m³):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.pipe_density = ttk.Entry(frame, width=15)
        self.pipe_density.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.pipe_density.insert(0, "7850")  # 钢的密度
        
        # 介质参数
        row += 1
        ttk.Label(frame, text="管道内介质密度 (kg/m³):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.media_density = ttk.Entry(frame, width=15)
        self.media_density.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="(非必填，用于计算运行重)").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        # 保温层参数
        row += 1
        ttk.Label(frame, text="保温层厚度 (mm):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.insulation_thickness = ttk.Entry(frame, width=15)
        self.insulation_thickness.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.insulation_thickness.insert(0, "50")
        
        row += 1
        ttk.Label(frame, text="保温材料密度 (kg/m³):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.insulation_density = ttk.Entry(frame, width=15)
        self.insulation_density.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.insulation_density.insert(0, "200")
        
        # 外保护层参数
        row += 1
        ttk.Label(frame, text="外保护层材料:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.cover_material = ttk.Combobox(frame, values=list(self.cover_materials.keys()), width=12)
        self.cover_material.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.cover_material.current(0)  # 默认选择钢板
        self.cover_material.bind("<<ComboboxSelected>>", self.update_cover_density)
        
        row += 1
        ttk.Label(frame, text="外保护层厚度 (mm):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.cover_thickness = ttk.Entry(frame, width=15)
        self.cover_thickness.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.cover_thickness.insert(0, "0.5")
        
        row += 1
        ttk.Label(frame, text="外保护层密度 (kg/m³):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.cover_density = ttk.Entry(frame, width=15)
        self.cover_density.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.cover_density.insert(0, str(self.cover_materials["钢板"]))
        
        # 支撑跨距计算参数
        row += 1
        ttk.Label(frame, text="管道允许挠度 (mm):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.deflection_limit = ttk.Entry(frame, width=15)
        self.deflection_limit.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.deflection_limit.insert(0, "20")
        
        row += 1
        ttk.Label(frame, text="管道材料弹性模量 (MPa):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.elastic_modulus = ttk.Entry(frame, width=15)
        self.elastic_modulus.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        self.elastic_modulus.insert(0, "206000")  # 钢的弹性模量
    
    def create_results_tab(self):
        # 创建一个框架来容纳所有结果
        frame = ttk.LabelFrame(self.tab_results, text="计算结果 (单位: kg/m 或 m)")
        frame.pack(fill="both", expand=True, padx=10, pady=10)
        
        # 使用网格布局排列结果
        row = 0
        
        ttk.Label(frame, text="管道自身重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_pipe_weight = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_pipe_weight.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="充水重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_water_weight = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_water_weight.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="介质运行重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_media_weight = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_media_weight.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="保温层重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_insulation_weight = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_insulation_weight.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="外保护层重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_cover_weight = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_cover_weight.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="总重量(含充水):").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_total_with_water = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_total_with_water.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Label(frame, text="运行总重量:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_total_operating = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_total_operating.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="kg/m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
        
        row += 1
        ttk.Separator(frame, orient="horizontal").grid(row=row, columnspan=3, sticky="ew", padx=5, pady=10)
        
        row += 1
        ttk.Label(frame, text="管道支撑允许跨距:").grid(row=row, column=0, sticky="w", padx=5, pady=5)
        self.result_span = ttk.Label(frame, text="--", background="white", padding=(5, 2), width=10)
        self.result_span.grid(row=row, column=1, sticky="w", padx=5, pady=5)
        ttk.Label(frame, text="m").grid(row=row, column=2, sticky="w", padx=5, pady=5)
    
    def update_cover_density(self, event):
        """根据选择的外保护层材料更新密度值"""
        selected_material = self.cover_material.get()
        density = self.cover_materials.get(selected_material, 0)
        
        # 清除当前值并设置新值
        self.cover_density.delete(0, tk.END)
        if density > 0:
            self.cover_density.insert(0, str(density))
        else:
            # 对于"其他"材料，让用户输入密度
            self.cover_density.focus_set()
    
    def calculate(self):
        try:
            # 获取输入参数
            outer_diameter = float(self.outer_diameter.get()) / 1000  # 转换为米
            wall_thickness = float(self.wall_thickness.get()) / 1000  # 转换为米
            pipe_density = float(self.pipe_density.get())
            
            # 获取介质密度（非必填）
            media_density = None
            if self.media_density.get().strip():
                media_density = float(self.media_density.get())
            
            insulation_thickness = float(self.insulation_thickness.get()) / 1000  # 转换为米
            insulation_density = float(self.insulation_density.get())
            
            cover_thickness = float(self.cover_thickness.get()) / 1000  # 转换为米
            cover_density = float(self.cover_density.get())
            
            deflection_limit = float(self.deflection_limit.get()) / 1000  # 转换为米
            elastic_modulus = float(self.elastic_modulus.get()) * 1e6  # 转换为Pa
            
            # 计算管道自身重量 (kg/m)
            inner_diameter = outer_diameter - 2 * wall_thickness
            pipe_cross_section = math.pi * (outer_diameter**2 - inner_diameter**2) / 4
            pipe_weight = pipe_cross_section * pipe_density
            
            # 计算充水重量 (kg/m) - 始终按照水计算
            water_cross_section = math.pi * (inner_diameter**2) / 4
            water_weight = water_cross_section * 1000  # 水的密度为1000 kg/m³
            
            # 计算介质运行重量 (kg/m) - 如果提供了介质密度
            media_weight = 0
            if media_density is not None:
                media_weight = water_cross_section * media_density
            
            # 计算保温层重量 (kg/m)
            insulation_outer_diameter = outer_diameter + 2 * insulation_thickness
            insulation_cross_section = math.pi * (insulation_outer_diameter**2 - outer_diameter**2) / 4
            insulation_weight = insulation_cross_section * insulation_density
            
            # 计算外保护层重量 (kg/m)
            cover_outer_diameter = insulation_outer_diameter + 2 * cover_thickness
            cover_cross_section = math.pi * (cover_outer_diameter**2 - insulation_outer_diameter**2) / 4
            cover_weight = cover_cross_section * cover_density
            
            # 计算总重量
            total_with_water = pipe_weight + water_weight + insulation_weight + cover_weight
            total_operating = pipe_weight + media_weight + insulation_weight + cover_weight
            
            # 计算管道支撑允许跨距 - 基于总重量(含充水)
            # 计算管道惯性矩
            inertia = math.pi * (outer_diameter**4 - inner_diameter**4) / 64
            
            # 简支梁跨中挠度公式: δ = 5 * q * L^4 / (384 * E * I)
            # 解出 L: L = (δ * 384 * E * I / (5 * q))^(1/4)
            # 其中 q 是总重量 (kg/m)，转换为 N/m 需乘以重力加速度9.81
            q = total_with_water * 9.81  # 转换为 N/m
            
            if q == 0:
                span = 0
            else:
                span = (deflection_limit * 384 * elastic_modulus * inertia / (5 * q)) **(1/4)
            
            # 更新结果显示
            self.result_pipe_weight.config(text=f"{pipe_weight:.2f}")
            self.result_water_weight.config(text=f"{water_weight:.2f}")
            self.result_media_weight.config(text=f"{media_weight:.2f}")
            self.result_insulation_weight.config(text=f"{insulation_weight:.2f}")
            self.result_cover_weight.config(text=f"{cover_weight:.2f}")
            self.result_total_with_water.config(text=f"{total_with_water:.2f}")
            self.result_total_operating.config(text=f"{total_operating:.2f}")
            self.result_span.config(text=f"{span:.2f}")
            
            # 切换到结果标签页
            self.tab_params.winfo_toplevel().nametowidget(self.tab_params.winfo_parent()).select(1)
            
        except ValueError as e:
            messagebox.showerror("输入错误", f"请确保所有输入都是有效的数字: {str(e)}")
        except Exception as e:
            messagebox.showerror("计算错误", f"计算过程中发生错误: {str(e)}")
    
    def reset(self):
        # 重置所有输入字段为默认值
        self.outer_diameter.delete(0, tk.END)
        self.outer_diameter.insert(0, "108")
        
        self.wall_thickness.delete(0, tk.END)
        self.wall_thickness.insert(0, "4")
        
        self.pipe_density.delete(0, tk.END)
        self.pipe_density.insert(0, "7850")
        
        self.media_density.delete(0, tk.END)  # 清空介质密度
        
        self.insulation_thickness.delete(0, tk.END)
        self.insulation_thickness.insert(0, "50")
        
        self.insulation_density.delete(0, tk.END)
        self.insulation_density.insert(0, "200")
        
        self.cover_material.current(0)  # 重置为钢板
        self.update_cover_density(None)  # 更新密度值
        
        self.cover_thickness.delete(0, tk.END)
        self.cover_thickness.insert(0, "0.5")
        
        self.deflection_limit.delete(0, tk.END)
        self.deflection_limit.insert(0, "20")
        
        self.elastic_modulus.delete(0, tk.END)
        self.elastic_modulus.insert(0, "206000")
        
        # 重置结果
        self.result_pipe_weight.config(text="--")
        self.result_water_weight.config(text="--")
        self.result_media_weight.config(text="--")
        self.result_insulation_weight.config(text="--")
        self.result_cover_weight.config(text="--")
        self.result_total_with_water.config(text="--")
        self.result_total_operating.config(text="--")
        self.result_span.config(text="--")
"""
if __name__ == "__main__":
    # 创建主窗口作为对话框的父窗口
    root = tk.Tk()
    root.withdraw()  # 隐藏主窗口
    
    # 创建并显示对话框
    dialog = PipeCalculatorDialog(root)
    dialog.wait_window()  # 等待对话框关闭
    
    # 对话框关闭后退出程序
    root.destroy()
  """  