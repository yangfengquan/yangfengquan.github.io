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

class App(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.root = root
        self.root.title("工程设计工具集")
        #self.root.geometry("1200x1000")
        self.root.state("zoomed")

        self._setup_menu()
        self._setup_ui()

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
        pipe_menu.add_command(label="管径计算", command=self.open_pipeFlowDialog)
        pipe_menu.add_command(label="管道阻力", command=self.open_pipeResistanceDialog)
        pipe_menu.add_command(label="管子重量", command=self.open_pipeWeightDialog)
        pipe_menu.add_command(label="管子壁厚", command=self.open_pipeThicknessDialog)
        pipe_menu.add_command(label="管子尺寸系列", command=self.open_pipeSizeSeriesDialog)
        pipe_menu.add_command(label="管道荷载", command=self.open_pipeLoadDialog)

        equipment_menu = tk.Menu(menubar, tearoff=0)
        equipment_menu.add_command(label="安全阀泄放面积", command=self.open_safetyValveDialog)
        equipment_menu.add_command(label="泵轴功率", command=self.open_pumpPowerDialog)

        # 创建帮助菜单
        help_menu = tk.Menu(menubar, tearoff=0)
        help_menu.add_command(label="帮助", command=self.app_help)
        help_menu.add_command(label="下载最新版本", command=self.open_download_url)
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