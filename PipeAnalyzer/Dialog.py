import tkinter as tk
from tkinter import ttk, messagebox, filedialog
class MaterialDialog(tk.Toplevel):
    """材料管理对话框"""
    
    def __init__(self, parent, material_manager, material_type):
        super().__init__(parent)
        self.material_manager = material_manager
        self.material_type = material_type
        
        self.title(f"{self.get_title()}管理")
        self.geometry("600x400")
        self.resizable(True, True)

        # 设置模态锁定，禁止操作父窗口
        self.grab_set()
        
        self.create_widgets()
        self.load_materials()

        # 等待对话框关闭后，再解除父窗口锁定
        parent.wait_window(self)
        self.grab_release()

    
    def get_title(self):
        """获取对话框标题"""
        titles = {
            'insulation': '保温材料',
            'protection': '外保护层',
            'fittings': '管道元件',
            'pipe_types': '管道类型'
        }
        return titles.get(self.material_type, '材料')
    
    def create_widgets(self):
        """创建界面组件"""
        # 主框架
        main_frame = ttk.Frame(self)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 列表框架
        list_frame = ttk.LabelFrame(main_frame, text=f"{self.get_title()}列表")
        list_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 10))
        
        # 创建树形视图
        columns = self.get_columns()
        self.tree = ttk.Treeview(list_frame, columns=columns, show='headings')
        
        # 设置列标题
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=100)
        
        # 滚动条
        scrollbar = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        self.tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 按钮框架
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(fill=tk.X)
        
        ttk.Button(button_frame, text="添加", command=self.add_material).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="编辑", command=self.edit_material).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="删除", command=self.delete_material).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="关闭", command=self.destroy).pack(side=tk.RIGHT, padx=5)
    
    def get_columns(self):
        """获取列定义"""
        if self.material_type == 'insulation':
            return ['名称', '导热系数方程一', '导热系数方程二', '导热系数方程三', '密度(kg/m³)', '描述'] 
        elif self.material_type == 'protection':
            return ['名称', '黑度', '描述']
        elif self.material_type == 'fittings':
            return ['名称', '阻力系数', '描述']
        else:  # pipe_types
            return ['名称', '粗糙度(m)', '描述'] 
    
    def load_materials(self):
        """加载材料数据"""
        # 清空现有数据
        for item in self.tree.get_children():
            self.tree.delete(item)
        
        # 获取材料字典
        if self.material_type == 'insulation':
            materials = self.material_manager.insulation_materials
        elif self.material_type == 'protection':
            materials = self.material_manager.protection_materials
        elif self.material_type == 'fittings':
            materials = self.material_manager.pipe_fittings
        else:  # pipe_types
            materials = self.material_manager.pipe_types
        
        # 添加数据
        for name, material in materials.items():
            if self.material_type == 'insulation':
                values = [
                    name,
                    material.conductivity_eq1 or '',
                    material.conductivity_eq2 or '',
                    material.conductivity_eq3 or '',
                    f"{material.density}",
                    material.description
                ]
            elif self.material_type == 'protection':
                values = [
                    name,
                    f"{material.emissivity:.3f}",
                    material.description
                ]
            elif self.material_type == 'fittings':
                values = [
                    name,
                    f"{material.resistance_coef:.3f}",
                    material.description
                ]
            else:  # pipe_types
                values = [
                    name,
                    f"{material.roughness:.6f}",
                    material.description
                ]
            
            self.tree.insert('', tk.END, values=values)
    
    def add_material(self):
        """添加材料"""
        dialog = MaterialEditDialog(self, self.material_manager, self.material_type)
        self.load_materials()
        # 通知父窗口更新下拉列表
        if hasattr(self.master, 'master') and hasattr(self.master.master, 'refresh_material_combos'):
            self.master.master.refresh_material_combos()
    
    def edit_material(self):
        """编辑材料"""
        selection = self.tree.selection()
        if not selection:
            messagebox.showwarning("警告", "请选择要编辑的材料")
            return
        
        item = selection[0]
        material_name = self.tree.item(item)['values'][0]
        
        dialog = MaterialEditDialog(self, self.material_manager, self.material_type, material_name)
        self.wait_window(dialog)
        self.load_materials()
        # 通知父窗口更新下拉列表
        if hasattr(self.master, 'master') and hasattr(self.master.master, 'refresh_material_combos'):
            self.master.master.refresh_material_combos()
    
    def delete_material(self):
        """删除材料"""
        selection = self.tree.selection()
        if not selection:
            messagebox.showwarning("警告", "请选择要删除的材料")
            return
        
        item = selection[0]
        material_name = self.tree.item(item)['values'][0]
        
        if messagebox.askyesno("确认", f"确定要删除材料 '{material_name}' 吗？"):
            if self.material_type == 'insulation':
                if material_name in self.material_manager.insulation_materials:
                    del self.material_manager.insulation_materials[material_name]
            elif self.material_type == 'protection':
                if material_name in self.material_manager.protection_materials:
                    del self.material_manager.protection_materials[material_name]
            elif self.material_type == 'fittings':
                if material_name in self.material_manager.pipe_fittings:
                    del self.material_manager.pipe_fittings[material_name]
            else:  # pipe_types
                if material_name in self.material_manager.pipe_types:
                    del self.material_manager.pipe_types[material_name]
            
            # 保存到文件
            self.material_manager.save_materials_to_file()
            self.load_materials()
            # 通知父窗口更新下拉列表
            if hasattr(self.master, 'master') and hasattr(self.master.master, 'refresh_material_combos'):
                self.master.master.refresh_material_combos()
    
class MaterialEditDialog(tk.Toplevel):
    """材料编辑对话框"""
    
    def __init__(self, parent, material_manager, material_type, material_name=None):
        super().__init__(parent)
        self.material_manager = material_manager
        self.material_type = material_type
        self.material_name = material_name
        self.is_edit = material_name is not None
        
        self.title(f"{'编辑' if self.is_edit else '添加'}{parent.get_title()}")
        self.geometry("500x400")
        self.resizable(False, False)

        # 设置模态锁定，禁止操作父窗口
        self.grab_set()

        self.create_widgets()
        if self.is_edit:
            self.load_material_data()
        
        # 等待对话框关闭后，再解除父窗口锁定
        parent.wait_window(self)
        self.grab_release()
    
    def create_widgets(self):
        """创建界面组件"""
        main_frame = ttk.Frame(self)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 名称
        ttk.Label(main_frame, text="名称:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.name_var = tk.StringVar()
        self.name_entry = ttk.Entry(main_frame, textvariable=self.name_var, width=30)
        self.name_entry.grid(row=0, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        
        if self.material_type == 'insulation':
            self.create_insulation_fields(main_frame)
        elif self.material_type == 'protection':
            self.create_protection_fields(main_frame)
        elif self.material_type == 'fittings':
            self.create_fittings_fields(main_frame)
        else:  # pipe_types
            self.create_pipe_type_fields(main_frame)
        
        # 按钮
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=10, column=0, columnspan=2, pady=20)
        
        ttk.Button(button_frame, text="保存", command=self.save_material).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="取消", command=self.destroy).pack(side=tk.LEFT, padx=5)
        
        main_frame.columnconfigure(1, weight=1)
    
    def create_insulation_fields(self, parent):
        """创建保温材料字段"""
        row = 1
        
        # 方程一
        ttk.Label(parent, text="导热系数方程一:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.conductivity_eq1_var = tk.StringVar()
        self.conductivity_eq1_entry = ttk.Entry(parent, textvariable=self.conductivity_eq1_var, width=30)
        self.conductivity_eq1_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1
        
        # 方程二
        ttk.Label(parent, text="导热系数方程二:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.conductivity_eq2_var = tk.StringVar()
        self.conductivity_eq2_entry = ttk.Entry(parent, textvariable=self.conductivity_eq2_var, width=30)
        self.conductivity_eq2_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1

        # 方程三
        ttk.Label(parent, text="导热系数方程三:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.conductivity_eq3_var = tk.StringVar()
        self.conductivity_eq3_entry = ttk.Entry(parent, textvariable=self.conductivity_eq3_var, width=30)
        self.conductivity_eq3_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1
        
        # 密度
        ttk.Label(parent, text="密度(kg/m³):").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.density_var = tk.StringVar(value="100")
        self.density_entry = ttk.Entry(parent, textvariable=self.density_var, width=30)
        self.density_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))     
        row += 1

        # 描述
        ttk.Label(parent, text="描述:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.description_var = tk.StringVar()
        self.description_entry = ttk.Entry(parent, textvariable=self.description_var, width=30)
        self.description_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
    
    def create_protection_fields(self, parent):
        """创建外保护层字段"""
        row = 1
        
        # 黑度
        ttk.Label(parent, text="黑度:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.emissivity_var = tk.StringVar(value="0.3")
        self.emissivity_entry = ttk.Entry(parent, textvariable=self.emissivity_var, width=30)
        self.emissivity_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1

        # 描述
        ttk.Label(parent, text="描述:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.description_var = tk.StringVar()
        self.description_entry = ttk.Entry(parent, textvariable=self.description_var, width=30)
        self.description_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
    
    def create_fittings_fields(self, parent):
        """创建管道元件字段"""
        row = 1
        
        # 阻力系数
        ttk.Label(parent, text="阻力系数:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.resistance_var = tk.StringVar(value="0.3")
        self.resistance_entry = ttk.Entry(parent, textvariable=self.resistance_var, width=30)
        self.resistance_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1
        
        # 描述
        ttk.Label(parent, text="描述:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.description_var = tk.StringVar()
        self.description_entry = ttk.Entry(parent, textvariable=self.description_var, width=30)
        self.description_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
    
    def create_pipe_type_fields(self, parent):
        """创建管道类型字段"""
        row = 1
        
        # 粗糙度
        ttk.Label(parent, text="粗糙度(m):").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.roughness_var = tk.StringVar(value="0.0002")
        self.roughness_entry = ttk.Entry(parent, textvariable=self.roughness_var, width=30)
        self.roughness_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
        row += 1
        
        # 描述
        ttk.Label(parent, text="描述:").grid(row=row, column=0, sticky=tk.W, pady=5)
        self.description_var = tk.StringVar()
        self.description_entry = ttk.Entry(parent, textvariable=self.description_var, width=30)
        self.description_entry.grid(row=row, column=1, sticky=tk.W+tk.E, pady=5, padx=(5, 0))
    
    def load_material_data(self):
        """加载材料数据"""
        if self.material_type == 'insulation':
            material = self.material_manager.insulation_materials.get(self.material_name)
            if material:
                self.name_var.set(material.name)
                self.conductivity_eq1_var.set(material.conductivity_eq1 or '')
                self.conductivity_eq2_var.set(material.conductivity_eq2 or '')
                self.conductivity_eq3_var.set(material.conductivity_eq3 or '')
                self.density_var.set(str(material.density))
                self.description_var.set(material.description)
        
        elif self.material_type == 'protection':
            material = self.material_manager.protection_materials.get(self.material_name)
            if material:
                self.name_var.set(material.name)
                self.emissivity_var.set(str(material.emissivity))
                self.description_var.set(material.description)
        
        elif self.material_type == 'fittings':
            material = self.material_manager.pipe_fittings.get(self.material_name)
            if material:
                self.name_var.set(material.name)
                self.resistance_var.set(str(material.resistance_coef))
                self.description_var.set(material.description)
        
        else:  # pipe_types
            material = self.material_manager.pipe_types.get(self.material_name)
            if material:
                self.name_var.set(material.name)
                self.roughness_var.set(str(material.roughness))
                self.description_var.set(material.description)
    
    def save_material(self):
        """保存材料"""
        name = self.name_var.get().strip()
        if not name:
            messagebox.showwarning("警告", "请输入材料名称")
            return
        
        try:
            if self.material_type == 'insulation':
                # 验证数据
                density = float(self.density_var.get())
                
                # 保存或更新
                if self.is_edit and name != self.material_name:
                    # 名称改变，删除旧记录
                    del self.material_manager.insulation_materials[self.material_name]
                
                self.material_manager.add_insulation_material(
                    name, 
                    self.conductivity_eq1_var.get(),
                    self.conductivity_eq3_var.get(),
                    self.conductivity_eq3_var.get(),
                    density,
                    self.description_var.get()
                )
            
            elif self.material_type == 'protection':
                emissivity = float(self.emissivity_var.get())
                
                if self.is_edit and name != self.material_name:
                    del self.material_manager.protection_materials[self.material_name]
                
                self.material_manager.add_protection_material(name, emissivity, self.description_var.get())  # 默认粗糙度和成本
            
            elif self.material_type == 'fittings':
                resistance_coef = float(self.resistance_var.get())
                description = self.description_var.get()
                
                if self.is_edit and name != self.material_name:
                    del self.material_manager.pipe_fittings[self.material_name]
                
                self.material_manager.add_pipe_fitting(name, resistance_coef, description)
            
            else:  # pipe_types
                roughness = float(self.roughness_var.get())
                description = self.description_var.get()
                
                if self.is_edit and name != self.material_name:
                    del self.material_manager.pipe_types[self.material_name]
                
                self.material_manager.add_pipe_type(name, roughness, description)  # 默认内径和壁厚
            
            # 保存到文件
            self.material_manager.save_materials_to_file()
            
            messagebox.showinfo("成功", f"材料 '{name}' 已保存")
            self.destroy()
            
        except ValueError as e:
            messagebox.showerror("错误", f"输入数据格式错误: {e}")