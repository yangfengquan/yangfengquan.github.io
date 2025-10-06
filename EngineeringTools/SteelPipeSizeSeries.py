import tkinter as tk
from tkinter import ttk
import math

class PipeDataManager:
    """管道数据管理类"""
    
    def __init__(self):
        self.DN = ["DN6", "DN8", "DN10", "DN15", "DN20", "DN25", "DN32", "DN40", "DN50", "DN65", 
                  "DN80", "DN90", "DN100", "DN125", "DN150", "DN200", "DN250", "DN300", "DN350", 
                  "DN400", "DN450", "DN500", "DN550", "DN600", "DN650", "DN700", "DN750", "DN800", 
                  "DN850", "DN900", "DN950", "DN1000", "DN1050", "DN1100", "DN1150", "DN1200"]
        
        self.D0 = {
            "DN6": 10.3, "DN8": 13.7, "DN10": 17.1, "DN15": 21.3, "DN20": 26.7, "DN25": 33.4,
            "DN32": 42.2, "DN40": 48.3, "DN50": 60.3, "DN65": 73.0, "DN80": 88.9, "DN90": 101.6,
            "DN100": 114.3, "DN125": 141.3, "DN150": 168.3, "DN200": 219.1, "DN250": 273.0,
            "DN300": 323.9, "DN350": 355.6, "DN400": 406.4, "DN450": 457.0, "DN500": 508.0,
            "DN550": 559.0, "DN600": 610.0, "DN650": 660.0, "DN700": 711.0, "DN750": 762.0,
            "DN800": 813.0, "DN850": 864.0, "DN900": 914.0, "DN950": 965.0, "DN1000": 1016.0,
            "DN1050": 1067.0, "DN1100": 1118.0, "DN1150": 1168.0, "DN1200": 1219.0
        }
        
        self.WALL_THICKNESS = {
            "DN6": {
                "SCH5": 1.24, "SCH10": 1.45, "SCH40": 1.73, "SCH80": 2.41,
                "STD": 1.73, "XS": 2.41, "SCH5S": 1.24, "SCH10S": 1.73, "SCH40S": 2.41
            },
            "DN8": {
                "SCH10": 1.65, "SCH30": 1.85, "SCH40": 2.24, "SCH80": 3.02,
                "STD": 2.24, "XS": 3.02, "SCH5S": 1.65, "SCH10S": 2.24, "SCH40S": 3.02
            },
            "DN10": {
                "SCH10": 1.65, "SCH30": 1.85, "SCH40": 2.31, "SCH80": 3.20,
                "STD": 2.31, "XS": 3.20, "SCH5S": 1.65, "SCH10S": 2.31, "SCH40S": 3.20
            },
            "DN15": {
                "SCH5": 1.65, "SCH10": 2.11, "SCH30": 2.41, "SCH40": 2.77, "SCH80": 3.73, "SCH160": 4.78,
                "STD": 2.77, "XS": 3.73, "XXS": 7.47, "SCH5S": 1.65, "SCH10S": 2.11, "SCH40S": 2.77, "SCH80S": 3.73
            },
            "DN20": {
                "SCH5": 1.65, "SCH10": 2.11, "SCH30": 2.41, "SCH40": 2.87, "SCH80": 3.91, "SCH160": 5.56,
                "XXS": 7.82, "SCH10S": 2.87, "SCH40S": 3.91
            },
            "DN25": {
                "SCH5": 1.65, "SCH10": 2.77, "SCH30": 2.91, "SCH40": 3.38, "SCH80": 4.55, "SCH160": 6.35,
                "STD": 3.38, "XS": 4.55, "XXS": 9.09, "SCH5S": 1.65, "SCH10S": 2.77, "SCH40S": 3.38, "SCH80S": 4.55
            },
            "DN32": {
                "SCH5": 1.65, "SCH10": 2.77, "SCH30": 2.97, "SCH40": 3.56, "SCH80": 4.85, "SCH160": 6.35,
                "STD": 3.56, "XS": 4.85, "XXS": 9.70, "SCH5S": 1.65, "SCH10S": 2.77, "SCH40S": 3.56, "SCH80S": 4.85
            },
            "DN40": {
                "SCH5": 1.65, "SCH10": 2.77, "SCH30": 3.18, "SCH40": 3.68, "SCH80": 5.08, "SCH160": 7.14,
                "STD": 3.68, "XS": 5.08, "XXS": 10.15, "SCH5S": 1.65, "SCH10S": 2.77, "SCH40S": 3.68, "SCH80S": 5.08
            },
            "DN50": {
                "SCH5": 1.65, "SCH10": 2.77, "SCH30": 3.18, "SCH40": 3.91, "SCH80": 5.54, "SCH160": 8.74,
                "STD": 3.91, "XS": 5.54, "XXS": 11.07, "SCH5S": 1.65, "SCH10S": 2.77, "SCH40S": 3.91, "SCH80S": 5.54
            },
            "DN65": {
                "SCH5": 2.11, "SCH10": 3.05, "SCH30": 4.78, "SCH40": 5.16, "SCH80": 7.01, "SCH160": 9.53,
                "STD": 5.16, "XS": 7.01, "XXS": 14.02, "SCH5S": 2.11, "SCH10S": 3.05, "SCH40S": 5.16, "SCH80S": 7.01
            },
            "DN80": {
                "SCH5": 2.11, "SCH10": 3.05, "SCH30": 4.78, "SCH40": 5.49, "SCH80": 7.62, "SCH160": 11.13,
                "STD": 5.49, "XS": 7.62, "XXS": 15.24, "SCH5S": 2.11, "SCH10S": 3.05, "SCH40S": 5.49, "SCH80S": 7.62
            },
            "DN90": {
                "SCH5": 2.11, "SCH10": 3.05, "SCH30": 4.78, "SCH40": 5.74, "SCH80": 8.08,
                "SCH5S": 2.11, "SCH10S": 3.05, "SCH40S": 5.74, "SCH80S": 8.08
            },
            "DN100": {
                "SCH5": 2.11, "SCH10": 3.05, "SCH30": 4.78, "SCH40": 6.02, "SCH80": 8.56, "SCH120": 11.13, "SCH160": 13.49,
                "STD": 6.02, "XS": 8.56, "XXS": 17.12, "SCH5S": 2.11, "SCH10S": 3.05, "SCH40S": 6.02, "SCH80S": 8.56
            },
            "DN125": {
                "SCH5": 2.77, "SCH10": 3.40, "SCH40": 6.55, "SCH80": 9.53, "SCH120": 12.70, "SCH160": 15.88,
                "STD": 6.55, "XS": 9.53, "XXS": 19.05, "SCH5S": 2.77, "SCH10S": 3.40, "SCH40S": 6.55, "SCH80S": 9.53
            },
            "DN150": {
                "SCH5": 2.77, "SCH10": 3.40, "SCH40": 7.11, "SCH80": 10.97, "SCH120": 12.70, "SCH160": 18.26,
                "STD": 7.11, "XS": 10.97, "XXS": 19.05, "SCH5S": 2.77, "SCH10S": 3.40, "SCH40S": 7.11, "SCH80S": 10.97
            },
            "DN200": {
                "SCH5": 2.77, "SCH10": 3.76, "SCH20": 6.35, "SCH30": 7.04, "SCH40": 8.18, "SCH60": 10.13, 
                "SCH80": 12.70, "SCH100": 15.09, "SCH120": 18.26, "SCH140": 20.62, "SCH160": 23.01,
                "STD": 8.18, "XS": 12.70, "XXS": 22.23, "SCH5S": 2.77, "SCH10S": 3.76, "SCH40S": 8.18, "SCH80S": 12.70
            },
            "DN250": {
                "SCH5": 3.40, "SCH10": 4.19, "SCH20": 6.35, "SCH30": 7.80, "SCH40": 9.27, "SCH60": 12.70, 
                "SCH80": 15.09, "SCH100": 18.26, "SCH120": 21.44, "SCH140": 25.40, "SCH160": 28.58,
                "STD": 9.27, "XS": 12.70, "XXS": 25.40, "SCH5S": 3.40, "SCH10S": 4.19, "SCH40S": 9.27, "SCH80S": 12.70
            },
            "DN300": {
                "SCH5": 3.96, "SCH10": 4.57, "SCH20": 6.35, "SCH30": 8.38, "SCH40": 10.31, "SCH60": 14.27, 
                "SCH80": 17.48, "SCH100": 21.44, "SCH120": 25.40, "SCH140": 28.58, "SCH160": 33.32,
                "STD": 9.53, "XS": 12.70, "XXS": 25.40, "SCH5S": 3.96, "SCH10S": 4.78, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN350": {
                "SCH5": 3.96, "SCH10": 6.35, "SCH20": 7.92, "SCH30": 9.53, "SCH40": 11.13, "SCH60": 15.09, 
                "SCH80": 19.05, "SCH100": 23.83, "SCH120": 27.79, "SCH140": 31.75, "SCH160": 35.71,
                "STD": 9.53, "XS": 12.70, "SCH5S": 3.96, "SCH10S": 4.78, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN400": {
                "SCH5": 4.19, "SCH10": 6.35, "SCH20": 7.92, "SCH30": 9.53, "SCH40": 12.70, "SCH60": 16.66, 
                "SCH80": 21.44, "SCH100": 26.19, "SCH120": 30.96, "SCH140": 36.53, "SCH160": 40.49,
                "STD": 9.53, "XS": 12.70, "SCH5S": 4.19, "SCH10S": 4.78, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN450": {
                "SCH5": 4.19, "SCH10": 6.35, "SCH20": 7.92, "SCH30": 11.13, "SCH40": 14.27, "SCH60": 19.05, 
                "SCH80": 23.83, "SCH100": 29.36, "SCH120": 34.93, "SCH140": 39.67, "SCH160": 45.24,
                "STD": 9.53, "XS": 12.70, "SCH5S": 4.19, "SCH10S": 4.78, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN500": {
                "SCH5": 4.78, "SCH10": 6.35, "SCH20": 9.53, "SCH30": 12.70, "SCH40": 15.09, "SCH60": 20.62, 
                "SCH80": 26.19, "SCH100": 32.54, "SCH120": 38.10, "SCH140": 44.45, "SCH160": 50.01,
                "STD": 9.53, "XS": 12.70, "SCH5S": 4.78, "SCH10S": 5.54, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN550": {
                "SCH5": 4.78, "SCH10": 6.35, "SCH20": 9.53, "SCH30": 12.70, "SCH60": 22.23, 
                "SCH80": 28.58, "SCH100": 34.93, "SCH120": 41.28, "SCH140": 47.63, "SCH160": 53.98,
                "STD": 9.53, "XS": 12.70, "SCH5S": 4.78, "SCH10S": 5.54, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN600": {
                "SCH5": 5.54, "SCH10": 6.35, "SCH20": 9.53, "SCH30": 14.27, "SCH40": 17.48, "SCH60": 24.61, 
                "SCH80": 30.96, "SCH100": 38.89, "SCH120": 46.02, "SCH140": 52.37, "SCH160": 59.54,
                "STD": 9.53, "XS": 12.70, "SCH5S": 5.54, "SCH10S": 6.35, "SCH40S": 9.53, "SCH80S": 12.70
            },
            "DN650": {"SCH10": 7.92, "SCH20": 12.70, "STD": 9.53, "XS": 12.70},
            "DN700": {"SCH10": 7.92, "SCH20": 12.70, "SCH30": 15.88, "STD": 9.53, "XS": 12.70},
            "DN750": {"SCH5": 6.35, "SCH10": 7.92, "SCH20": 12.70,  "SCH30": 15.88, "STD": 9.53, "XS": 12.70, "SCH5S": 6.35, "SCH10S": 7.92},
            "DN800": {"SCH10": 7.92, "SCH20": 12.70, "SCH30": 15.88, "SCH40": 17.48, "STD": 9.53, "XS": 12.70},
            "DN850": {"SCH10": 7.92, "SCH20": 12.70, "SCH30": 15.88, "SCH40": 17.48, "STD": 9.53, "XS": 12.70},
            "DN900": {"SCH10": 7.92, "SCH20": 12.70, "SCH30": 15.88, "SCH40": 19.05, "STD": 9.53, "XS": 12.70},
            "DN950": {"STD": 9.53, "XS": 12.70},
            "DN1000": {"STD": 9.53, "XS": 12.70},
            "DN1050": {"STD": 9.53, "XS": 12.70},
            "DN1100": {"STD": 9.53, "XS": 12.70},
            "DN1150": {"STD": 9.53, "XS": 12.70},
            "DN1200": {"STD": 9.53, "XS": 12.70}
        }
    
    def get_wall_thickness(self, dn: str, schedule: str) -> float:
        """获取指定管道尺寸和规格的壁厚"""
        if dn not in self.WALL_THICKNESS:
            raise ValueError(f"不支持的管道尺寸: {dn}")
        
        if schedule not in self.WALL_THICKNESS[dn]:
            raise ValueError(f"管道尺寸 {dn} 不支持规格: {schedule}")
        
        return self.WALL_THICKNESS[dn][schedule]
    
    def get_outer_diameter(self, dn: str) -> float:
        """获取管道外径"""
        if dn not in self.D0:
            raise ValueError(f"不支持的管道尺寸: {dn}")
        return self.D0[dn]
    
    def get_available_schedules(self, dn: str) -> list:
        """获取指定管道尺寸可用的所有规格"""
        if dn not in self.WALL_THICKNESS:
            return []
        return list(self.WALL_THICKNESS[dn].keys())
    
    def calculate_pipe_weight(self, dn: str, schedule: str, density: float = 7.85) -> float:
        """计算管道理论质量 (kg/m)"""
        outer_diameter = self.get_outer_diameter(dn)  # mm
        thickness = self.get_wall_thickness(dn, schedule)  # mm
        
        # 计算内径
        inner_diameter = outer_diameter - 2 * thickness
        
        # 计算截面积 (mm²)
        cross_sectional_area = math.pi * (outer_diameter**2 - inner_diameter**2) / 4
        
        # 计算理论质量 (kg/m)
        weight_per_meter = cross_sectional_area * density / 1000
        
        return round(weight_per_meter, 2)
    
    def get_all_pipe_data(self, dn: str) -> list:
        """获取指定公称直径的所有管道数据"""
        if dn not in self.WALL_THICKNESS:
            return []
        
        data = []
        available_schedules = self.get_available_schedules(dn)
        outer_diameter = self.get_outer_diameter(dn)
        
        for schedule in available_schedules:
            thickness = self.get_wall_thickness(dn, schedule)
            weight = self.calculate_pipe_weight(dn, schedule)
            data.append({
                'schedule': schedule,
                'outer_diameter': outer_diameter,
                'thickness': thickness,
                'weight': weight
            })
        
        return data


class PipeSizeSeriesDialog(tk.Toplevel):
    """管道选择对话框"""
    
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent
        self.pipe_data = PipeDataManager()
        
        self.title("钢管尺寸系列 - SH/T3035-2017")
        self.geometry("800x600")
        self.resizable(True, True)
        
        # 设置图标（如果有的话）
        try:
            self.iconbitmap("pipe_icon.ico")  # 可选
        except:
            pass
        
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
        # 主框架
        main_frame = ttk.Frame(self, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 配置网格权重
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        main_frame.rowconfigure(3, weight=1)
        
        # 标题
        title_label = ttk.Label(main_frame, text="碳素钢、合金钢无缝钢管及焊接钢管的尺寸和理论质量", 
                               font=("Arial", 12, "bold"))
        title_label.grid(row=0, column=0, columnspan=2, pady=(0, 15))
        
        # 公称直径选择
        ttk.Label(main_frame, text="公称直径 (DN):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.dn_var = tk.StringVar()
        self.dn_combo = ttk.Combobox(main_frame, textvariable=self.dn_var, values=self.pipe_data.DN, 
                                    state="readonly", width=15)
        self.dn_combo.grid(row=1, column=1, sticky=(tk.W, tk.E), pady=5, padx=(5, 0))
        self.dn_combo.bind('<<ComboboxSelected>>', self.on_dn_selected)
        
        # 管道规格选择
        ttk.Label(main_frame, text="管道规格:").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.schedule_var = tk.StringVar()
        self.schedule_combo = ttk.Combobox(main_frame, textvariable=self.schedule_var, 
                                          state="readonly", width=15)
        self.schedule_combo.grid(row=2, column=1, sticky=(tk.W, tk.E), pady=5, padx=(5, 0))
        self.schedule_combo.bind('<<ComboboxSelected>>', self.on_schedule_selected)
        
        # 结果显示区域
        result_frame = ttk.LabelFrame(main_frame, text="管道参数详情", padding="5")
        result_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(15, 0))
        result_frame.columnconfigure(0, weight=1)
        result_frame.rowconfigure(0, weight=1)
        
        # 创建Treeview来显示数据
        columns = ('schedule', 'outer_diameter', 'thickness', 'weight')
        self.tree = ttk.Treeview(result_frame, columns=columns, show='headings', height=15)
        
        # 定义列
        self.tree.heading('schedule', text='管道规格')
        self.tree.heading('outer_diameter', text='外径 (mm)')
        self.tree.heading('thickness', text='壁厚 (mm)')
        self.tree.heading('weight', text='单位质量 (kg/m)')
        
        # 设置列宽
        self.tree.column('schedule', width=120)
        self.tree.column('outer_diameter', width=100)
        self.tree.column('thickness', width=100)
        self.tree.column('weight', width=120)
        
        # 添加滚动条
        scrollbar = ttk.Scrollbar(result_frame, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        self.tree.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        
        # 底部按钮框架
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=4, column=0, columnspan=2, pady=(15, 0))
        
        ttk.Button(button_frame, text="刷新数据", command=self.refresh_data).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="导出数据", command=self.export_data).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="关闭", command=self.destroy).pack(side=tk.RIGHT, padx=5)
        
        # 状态栏
        self.status_var = tk.StringVar(value="请选择公称直径")
        status_label = ttk.Label(main_frame, textvariable=self.status_var, relief=tk.SUNKEN)
        status_label.grid(row=5, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(10, 0))
    
    def on_dn_selected(self, event=None):
        """公称直径选择事件"""
        selected_dn = self.dn_var.get()
        if selected_dn:
            # 更新管道规格下拉列表
            available_schedules = self.pipe_data.get_available_schedules(selected_dn)
            self.schedule_combo['values'] = available_schedules
            
            # 显示所有规格的数据
            self.display_all_data(selected_dn)
            
            self.status_var.set(f"已选择: {selected_dn}, 共 {len(available_schedules)} 种规格")
    
    def on_schedule_selected(self, event=None):
        """管道规格选择事件"""
        selected_dn = self.dn_var.get()
        selected_schedule = self.schedule_var.get()
        
        if selected_dn and selected_schedule:
            try:
                # 高亮显示选中的规格
                for item in self.tree.get_children():
                    if self.tree.set(item, 'schedule') == selected_schedule:
                        self.tree.selection_set(item)
                        self.tree.focus(item)
                        break
                
                self.status_var.set(f"{selected_dn} - {selected_schedule} 已选中")
            except Exception as e:
                self.status_var.set(f"错误: {str(e)}")
    
    def display_all_data(self, dn: str):
        """显示指定公称直径的所有管道数据"""
        # 清空现有数据
        for item in self.tree.get_children():
            self.tree.delete(item)
        
        # 获取并显示新数据
        pipe_data = self.pipe_data.get_all_pipe_data(dn)
        
        for data in pipe_data:
            self.tree.insert('', tk.END, values=(
                data['schedule'],
                data['outer_diameter'],
                data['thickness'],
                data['weight']
            ))
    
    def refresh_data(self):
        """刷新数据"""
        selected_dn = self.dn_var.get()
        if selected_dn:
            self.display_all_data(selected_dn)
            self.status_var.set(f"数据已刷新 - {selected_dn}")
    
    def export_data(self):
        """导出数据（示例功能）"""
        selected_dn = self.dn_var.get()
        if selected_dn:
            # 这里可以添加导出到Excel或CSV的功能
            self.status_var.set(f"导出功能开发中 - {selected_dn}")
        else:
            self.status_var.set("请先选择公称直径")


# 使用示例
if __name__ == "__main__":
    root = tk.Tk()
    root.withdraw()  # 隐藏主窗口
    
    def show_dialog():
        dialog = PipeSizeSeriesDialog(root)
        dialog.grab_set()  # 模态对话框
        root.wait_window(dialog)
    
    # 创建一个简单的启动按钮
    launch_btn = tk.Button(root, text="打开管道参数查询", command=show_dialog)
    launch_btn.pack(pady=20)
    
    root.deiconify()
    root.mainloop()