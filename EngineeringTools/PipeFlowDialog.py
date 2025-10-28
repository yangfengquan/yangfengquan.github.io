import tkinter as tk
from tkinter import ttk, messagebox
import math

class PipeFlowDialog:
    # 常用流速参考数据（四列布局）
    FLOW_REFERENCES = [
        ("水", "1-3", "海水", "1.5-2.5"),
        ("热水", "1-2", "冷却水", "1.5-3"),
        ("压缩空气", "8-15", "氧气", "8-10"),
        ("氮气", "10-15", "氢气", "15-20"),
        ("饱和蒸汽", "20-40", "过热蒸汽", "30-50"),
        ("一般油品", "0.5-2", "高粘度油品", "0.2-1"),
        ("天然气", "10-20", "液化石油气", "8-15"),
        ("真空管路", "0.5-1.5", "蒸汽冷凝水", "0.5-1.5")
    ]

    def __init__(self, parent):
        # 创建顶层窗口并设置基本属性
        self.dialog = tk.Toplevel(parent)
        self.dialog.title("管道流速计算器")
        self.dialog.geometry("520x450")
        self.dialog.resizable(False, False)
        
        # 存储计算结果和输入变量
        self.result = None
        self._init_variables()
        
        # 设置UI并布局
        self._setup_ui()
        
        # 模态设置
        self.dialog.grab_set()
        parent.wait_window(self.dialog)
    
    def _init_variables(self):
        """初始化输入变量"""
        self.pipe_diameter = tk.StringVar()  # 管道内径(mm)
        self.flow_rate = tk.StringVar()      # 流量(m³/h)
        self.flow_speed = tk.StringVar()     # 流速(m/s)
    
    def _setup_ui(self):
        """设置用户界面"""
        # 创建主框架，统一管理内边距
        main_frame = ttk.Frame(self.dialog, padding=(20, 15))
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 标题标签
        self._create_title(main_frame)
        
        # 输入区域
        self._create_input_fields(main_frame)
        
        # 参考数据区域
        self._create_reference_section(main_frame)
        
        # 结果显示区域
        self._create_result_display(main_frame)
        
        # 按钮区域
        self._create_buttons(main_frame)
    
    def _create_title(self, parent):
        """创建标题标签"""
        title_label = ttk.Label(
            parent, 
            text="管道流速计算", 
            font=("SimHei", 14, "bold"),
            foreground="#333333"
        )
        title_label.grid(row=0, column=0, columnspan=2, pady=(0, 15))
        
        # 简化说明文本
        desc_label = ttk.Label(
            parent, 
            text="输入两个参数，计算第三个参数", 
            font=("SimHei", 10),
            foreground="#666666",
            justify=tk.CENTER
        )
        desc_label.grid(row=1, column=0, columnspan=2, pady=(0, 12))
    
    def _create_input_fields(self, parent):
        """创建输入字段"""
        # 管道内径输入
        ttk.Label(parent, text="管道内径(mm)：", font=("SimHei", 10)).grid(
            row=2, column=0, sticky=tk.W, pady=6)
        ttk.Entry(
            parent, 
            textvariable=self.pipe_diameter, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=2, column=1, pady=6)
        
        # 流量输入
        ttk.Label(parent, text="流量(m³/h)：", font=("SimHei", 10)).grid(
            row=3, column=0, sticky=tk.W, pady=6)
        ttk.Entry(
            parent, 
            textvariable=self.flow_rate, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=3, column=1, pady=6)
        
        # 流速输入
        ttk.Label(parent, text="流速(m/s)：", font=("SimHei", 10)).grid(
            row=4, column=0, sticky=tk.W, pady=6)
        ttk.Entry(
            parent, 
            textvariable=self.flow_speed, 
            width=20,
            font=("SimHei", 10)
        ).grid(row=4, column=1, pady=6)
    
    def _create_reference_section(self, parent):
        """创建参考数据区域（四列布局）"""
        ref_frame = ttk.LabelFrame(parent, text="常用流速参考 (m/s)", padding=10)
        ref_frame.grid(row=5, column=0, columnspan=2, pady=8, sticky=tk.W+tk.E)
        
        # 循环添加参考数据
        for row, (m1, s1, m2, s2) in enumerate(self.FLOW_REFERENCES):
            ttk.Label(ref_frame, text=m1, font=("SimHei", 9), foreground="#555555").grid(
                row=row, column=0, sticky=tk.W, padx=(5, 20), pady=1)
            ttk.Label(ref_frame, text=s1, font=("SimHei", 9), foreground="#555555").grid(
                row=row, column=1, sticky=tk.W, padx=(0, 30), pady=1)
            ttk.Label(ref_frame, text=m2, font=("SimHei", 9), foreground="#555555").grid(
                row=row, column=2, sticky=tk.W, padx=(5, 20), pady=1)
            ttk.Label(ref_frame, text=s2, font=("SimHei", 9), foreground="#555555").grid(
                row=row, column=3, sticky=tk.W, pady=1)
    
    def _create_result_display(self, parent):
        """创建结果显示区域"""
        self.result_label = ttk.Label(
            parent, 
            text="计算结果将显示在这里", 
            font=("SimHei", 10),
            foreground="#666666",
            justify=tk.CENTER
        )
        self.result_label.grid(row=6, column=0, columnspan=2, pady=8)
    
    def _create_buttons(self, parent):
        """创建按钮区域"""
        btn_frame = ttk.Frame(parent)
        btn_frame.grid(row=7, column=0, columnspan=2, pady=5)
        
        # 计算按钮
        ttk.Button(btn_frame, text="计算", command=self.calculate).pack(side=tk.LEFT, padx=10)
        
        # 清空按钮
        ttk.Button(btn_frame, text="清空", command=self.clear_fields).pack(side=tk.LEFT, padx=10)
        
        # 确认按钮
        ttk.Button(btn_frame, text="确认", command=self._on_confirm).pack(side=tk.LEFT, padx=10)
        
        # 取消按钮
        ttk.Button(btn_frame, text="取消", command=self._on_cancel).pack(side=tk.LEFT, padx=10)
    
    def calculate(self):
        """根据输入的两个参数计算第三个参数"""
        try:
            # 获取并验证输入
            inputs = self._get_and_validate_inputs()
            if not inputs:
                return
                
            # 根据输入参数计算结果
            result_text = self._perform_calculation(inputs)
            
            # 更新结果显示
            self.result_label.config(
                text=result_text,
                foreground="#0066cc",
                font=("SimHei", 11, "bold")
            )
            
        except ValueError:
            messagebox.showerror("输入错误", "请输入有效的数字")
        except Exception as e:
            messagebox.showerror("错误", f"计算出错：{str(e)}")
    
    def _get_and_validate_inputs(self):
        """获取并验证输入值"""
        # 获取输入值
        inputs = {
            'diameter': self.pipe_diameter.get().strip(),
            'rate': self.flow_rate.get().strip(),
            'speed': self.flow_speed.get().strip()
        }
        
        # 统计非空输入数量
        non_empty = sum(1 for val in inputs.values() if val)
        if non_empty != 2:
            messagebox.showerror("输入错误", "请输入且仅输入两个参数")
            return None
        
        # 转换为数值并验证正数
        try:
            for key, val in inputs.items():
                if val:
                    inputs[key] = float(val)
                    if inputs[key] <= 0:
                        messagebox.showerror("输入错误", "所有参数必须大于0")
                        return None
                else:
                    inputs[key] = None
            return inputs
        except ValueError:
            raise
    
    def _perform_calculation(self, inputs):
        """执行具体的计算逻辑"""
        diameter, rate, speed = inputs['diameter'], inputs['rate'], inputs['speed']
        
        if diameter is not None and rate is not None:
            # 计算流速
            speed = rate * 4 * 1000000 / (math.pi * diameter**2 * 3600)
            speed = round(speed, 2)
            self.flow_speed.set(str(speed))
            self.result = {'diameter': diameter, 'rate': rate, 'speed': speed}
            return f"计算完成：流速 = {speed} m/s"
            
        elif diameter is not None and speed is not None:
            # 计算流量
            rate = speed * math.pi * diameter**2 * 3600 / (4 * 1000000)
            rate = round(rate, 2)
            self.flow_rate.set(str(rate))
            self.result = {'diameter': diameter, 'rate': rate, 'speed': speed}
            return f"计算完成：流量 = {rate} m³/h"
            
        elif rate is not None and speed is not None:
            # 计算内径
            diameter = math.sqrt(rate * 4 * 1000000 / (math.pi * speed * 3600))
            diameter = round(diameter, 1)
            self.pipe_diameter.set(str(diameter))
            self.result = {'diameter': diameter, 'rate': rate, 'speed': speed}
            return f"计算完成：内径 = {diameter} mm"
    
    def clear_fields(self):
        """清空所有输入框"""
        self.pipe_diameter.set("")
        self.flow_rate.set("")
        self.flow_speed.set("")
        self.result_label.config(
            text="计算结果将显示在这里",
            foreground="#666666",
            font=("SimHei", 10)
        )
        self.result = None
    
    def _on_confirm(self):
        """确认按钮回调"""
        if self.result is None:
            messagebox.showinfo("提示", "请先进行计算")
            return
        self.dialog.destroy()
    
    def _on_cancel(self):
        """取消按钮回调"""
        self.result = None
        self.dialog.destroy()


# 测试代码
# if __name__ == "__main__":
#     root = tk.Tk()
#     root.title("管道流速计算器测试")
#     root.geometry("300x200")
    
#     def open_calculator():
#         """打开管道流速计算器"""
#         dialog = PipeFlowDialog(root)
#         if dialog.result:
#             result = dialog.result
#             messagebox.showinfo(
#                 "计算结果", 
#                 f"内径：{result['diameter']} mm\n"
#                 f"流量：{result['rate']} m³/h\n"
#                 f"流速：{result['speed']} m/s"
#             )
    
#     # 添加打开计算器的按钮
#     ttk.Button(
#         root, 
#         text="打开管道流速计算器", 
#         command=open_calculator
#     ).pack(expand=True)
    
#     root.mainloop()
    