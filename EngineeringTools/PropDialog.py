import tkinter as tk
from tkinter import ttk, messagebox
from EditableTreeview import EditableTreeview
import CoolProp.CoolProp as CoolProp
import re
class PropDialog:
    def __init__(self, parent, title="物质物性"):
        self.dialog = tk.Toplevel(parent)
        self.dialog.title(title)
        self.dialog.geometry("500x800")

        self._setup_ui()
        
        self.init_component_list_tree()
        self.init_res_tree()

    def _setup_ui(self):
        ttk.Label(self.dialog, text="组分：").grid(row=0, column=0, padx=10, pady=5, sticky=tk.W)
        self.component_entry_var = tk.StringVar()
        component_entry = ttk.Entry(self.dialog, textvariable=self.component_entry_var, width=57)
        component_entry.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        component_entry.bind('<KeyRelease>', self.component_entry_changed)

        components_tree_frame = tk.Frame(self.dialog, )
        components_tree_frame.grid(row=1, column=1, padx=10, pady=5, sticky=tk.W)
        self.components_tree = ttk.Treeview(components_tree_frame, columns=("name","formula","CAS"), show="headings")
        self.components_tree.heading("name", text="名称")
        self.components_tree.heading("formula", text="化学式")
        self.components_tree.heading("CAS", text="CAS")
        self.components_tree.column('name', width=200, stretch=True)
        self.components_tree.column('formula', width=100, stretch=False)
        self.components_tree.column('CAS', width=100, stretch=False)
        components_tree_scrollbar = ttk.Scrollbar(components_tree_frame, orient=tk.VERTICAL, command=self.components_tree.yview)
        self.components_tree.configure(yscrollcommand=components_tree_scrollbar.set)
        self.components_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        components_tree_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.components_tree.bind("<Double-1>", self.add_component)

        btn_frame = tk.Frame(self.dialog, )
        btn_frame.grid(row=2, column=1, sticky=tk.W)
        ttk.Button(btn_frame, text="添加组分>>", command=self.add_component).grid(row=0, column=0, padx=10, pady=5, sticky=tk.W)
        ttk.Button(btn_frame, text="<<删除组分", command=self.delete_component).grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        
        components_selected_tree_frame = tk.Frame(self.dialog, )
        components_selected_tree_frame.grid(row=3, column=1, padx=10, pady=5, sticky=tk.W)
        self.components_selected_tree = EditableTreeview(
            components_selected_tree_frame,
            editable_columns=['fraction'],
            columns=('name','fraction'),
            show="headings",
            height=5)
        self.components_selected_tree.heading('name', text="名称")
        self.components_selected_tree.heading('fraction', text="摩尔分数")
        self.components_selected_tree.column('name', width=200, stretch=False)
        self.components_selected_tree.column('fraction', width=200, stretch=True)
        components_selected_tree_scrollbar = ttk.Scrollbar(components_selected_tree_frame, orient=tk.VERTICAL, command=self.components_selected_tree.yview)
        self.components_selected_tree.configure(yscrollcommand=components_selected_tree_scrollbar.set)
        self.components_selected_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        components_selected_tree_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
       
        ttk.Label(self.dialog, text="参数1：").grid(row=4, column=0, padx=10, pady=5, sticky=tk.W)
        arg1_frame = tk.Frame(self.dialog, )
        arg1_frame.grid(row=4, column=1, sticky=tk.W)
        self.arg1_combo_var = tk.StringVar()
        arg1_combo = ttk.Combobox(arg1_frame, textvariable=self.arg1_combo_var, values=["压力", "温度", "干度"])
        arg1_combo.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        arg1_combo.bind('<<ComboboxSelected>>', self.arg1_combo_selected)

        self.arg1_entry_var = tk.StringVar()
        arg1_entry = ttk.Entry(arg1_frame, textvariable=self.arg1_entry_var)
        arg1_entry.grid(row=0, column=2, padx=10, pady=5, sticky=tk.W)

        self.arg1_unit_var = tk.StringVar()
        arg1_unit_label = ttk.Label(arg1_frame, textvariable=self.arg1_unit_var)
        arg1_unit_label.grid(row=0, column=3, padx=0, pady=5, sticky=tk.W)
        #初始值，测试
        #self.arg1_unit_var.set("MPa")

        ttk.Label(self.dialog, text="参数2：").grid(row=5, column=0, padx=10, pady=5, sticky=tk.W)
        arg2_frame = tk.Frame(self.dialog, )
        arg2_frame.grid(row=5, column=1, sticky=tk.W)
        self.arg2_combo_var = tk.StringVar()
        arg2_combo = ttk.Combobox(arg2_frame, textvariable=self.arg2_combo_var, values=["温度", "干度"])
        arg2_combo.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        arg2_combo.bind('<<ComboboxSelected>>', self.arg2_combo_selected)

        self.arg2_entry_var = tk.StringVar()
        arg2_entry = ttk.Entry(arg2_frame, textvariable=self.arg2_entry_var)
        arg2_entry.grid(row=0, column=2, padx=10, pady=5, sticky=tk.W)

        self.arg2_unit_var = tk.StringVar()
        arg2_unit_label = ttk.Label(arg2_frame, textvariable=self.arg2_unit_var)
        arg2_unit_label.grid(row=0, column=3, padx=0, pady=5, sticky=tk.W)

        ttk.Button(self.dialog, text="运算", command=self.calc).grid(row=6, column=1, padx=10, pady=5, sticky=(tk.W, tk.N))

        ttk.Label(self.dialog, text="结果：").grid(row=7, column=0, padx=10, pady=5, sticky=(tk.W, tk.N))
        res_tree_frame = tk.Frame(self.dialog, )
        res_tree_frame.grid(row=7, column=1, padx=10, pady=5, sticky=tk.W)
        self.res_tree = ttk.Treeview(res_tree_frame, columns=("name","unit","value"), show="headings")
        self.res_tree.heading("name", text="名称")
        self.res_tree.heading("unit", text="单位")
        self.res_tree.heading("value", text="数值")
        self.res_tree.column('name', width=200, stretch=True)
        self.res_tree.column('unit', width=100, stretch=True)
        self.res_tree.column('value', width=100, stretch=True)
        res_tree_scrollbar = ttk.Scrollbar(res_tree_frame, orient=tk.VERTICAL, command=self.res_tree.yview)
        self.res_tree.configure(yscrollcommand=res_tree_scrollbar.set)
        self.res_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        res_tree_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.properties = {
            'P': ('压力', 'MPa'),
            'T': ('温度', 'C'),
            'Q': ('干度', ''),
            'M':('摩尔质量', 'kg/mol'),
            'DMOLAR': ('摩尔密度', 'mole/m3'),
            'DMASS': ('质量密度', 'kg/m3'),
            'HMOLAR':('摩尔比焓', 'J/mol'),
            'HMASS':('质量比焓', 'J/kg'),
            'SMOLAR':('摩尔比熵', 'J/mol.K'),
            'SMASS':('质量比熵', 'J/kg.K'),
            'UMOLAR':('摩尔内能', 'J/mol'),
            'UMASS':('质量内能', 'J/kg'),
            'CPMOLAR':('摩尔定压比热', 'J/mol.k'),
            'CPMASS':('质量定压比热', 'J/kg.k'),
            'CVMOLAR':('摩尔定容比热', 'J/mol.k'),
            'CVMASS':('质量定容比热', 'J/kg.k'),
            'GAS_CONSTANT':('摩尔气体常熟', 'J/mol.k'),
            'ISENTROPIC_EXPANSION_COEFFICIENT':('绝热膨胀系数', ''),
            'ISOBARIC_EXPANSION_COEFFICIENT':('等压膨胀系数', '1/K'),
            'ISOTHERMAL_COMPRESSIBILITY':('等温压缩系数', '1/Pa'),
            'CONDUCTIVITY':('热导率', 'W/m.K'),
            'A':('声速', 'm/s'),
            'I':('表面张力', 'N/m'),
            'V':('运动粘度', 'Pa.s'),
            'Z':('压缩系数', '')
        }

        
    def init_component_list_tree(self):
        fliuids_list = CoolProp.FluidsList()
        components = []
        for item in fliuids_list:
            components.append((item, re.sub(r'[_,{,},1]','', CoolProp.get_fluid_param_string(item,"formula")), CoolProp.get_fluid_param_string(item,"CAS")))
        for v in components:
            self.components_tree.insert('', 'end', values=v)

    def init_res_tree(self):
        for param in self.properties.keys():
            self.res_tree.insert('', 'end', values=(self.properties[param][0],self.properties[param][1]))
    
    def component_entry_changed(self, event):
        input = self.component_entry_var.get().replace(" ","")
        for item in self.components_tree.get_children():
            self.components_tree.delete(item)
        if len(input) == 0:
            self.init_component_list_tree()
            return
        matched_component_list = []
        fliuids_list = CoolProp.FluidsList()
        components = []
        for item in fliuids_list:
            components.append((item, re.sub(r'[_,{,},1]','', CoolProp.get_fluid_param_string(item,"formula")), CoolProp.get_fluid_param_string(item,"CAS")))
        for item in components:
            if input.lower() in (''.join(item)).lower():
                matched_component_list.append(item)

        if len(matched_component_list):
            for v in matched_component_list:
                self.components_tree.insert('', 'end', values=v)
    
    def add_component(self, event=None):
        selected_item = self.components_tree.selection()[0]
        data = self.components_tree.item(selected_item, "values")
        self.components_selected_tree.insert('', 'end', values=(data[0]))

    def delete_component(self):
        selected_items = self.components_selected_tree.selection()
        if selected_items:
            self.components_selected_tree.delete(selected_items[0])
    
    def arg1_combo_selected(self, event):
        units = {'压力': 'MPa', '温度': 'C', '干度': ''}
        self.arg1_unit_var.set(units[self.arg1_combo_var.get()])
        if self.arg1_combo_var.get() == self.arg2_combo_var.get():
            messagebox.showwarning(title="警告", message="参数1与参数2不得选择同一参数。")

    def arg2_combo_selected(self, event):
        units = {'压力': 'MPa', '温度': 'C', '干度': ''}
        self.arg2_unit_var.set(units[self.arg2_combo_var.get()])
        if self.arg1_combo_var.get() == self.arg2_combo_var.get():
            messagebox.showwarning(title="警告", message="参数1与参数2不得选择同一参数。")
    
    def calc(self):
        components = []
        fractions = []
        for item in self.components_selected_tree.get_children():
            try:
                components.append(self.components_selected_tree.item(item, "values")[0])
                fractions.append(float(self.components_selected_tree.item(item, "values")[1]))
            except IndexError:
                if len(components) > 1:
                    messagebox.showwarning("警告", message="摩尔分数未输入。")
                    return
        if len(components) > 1 and abs(sum(fractions) - 1.0) > 1e-10:
            messagebox.showwarning(title="警告", message="摩尔分数总和必须为1。")
        
        def _create_mixture_string():
            """创建混合物字符串"""
            if len(components) == 1:
                return components[0]
            components_with_fractions = []
            for comp, frac in zip(components, fractions):
                components_with_fractions.append(f"{comp}[{frac}]")
            return "&".join(components_with_fractions)
        
        param1 = self.arg1_combo_var.get()
        if param1 == '':
            messagebox.showwarning(title="警告", message="请对参数1进行选择。")
            return
        if param1 == '压力':
            _param1 = 'P'
        elif param1 == '温度':
            _param1 = 'T'
        elif param1 == '干度':
            _param1 = 'Q'

        try:
            value1 = float(self.arg1_entry_var.get())
        except ValueError:
            messagebox.showwarning(title="警告", message="参数1输入不正确。")
            return

        if _param1 == 'P':
            value1 = value1 * 1e6
        if _param1 == 'T':
            value1 += 273.15
        
        param2 = self.arg2_combo_var.get()
        if param2 == '':
            messagebox.showwarning(title="警告", message="请对参数2进行选择。")
            return
        if param2 == '温度':
            _param2 = 'T'
        elif param2 == '干度':
            _param2 = 'Q'

        try:
            value2 = float(self.arg2_entry_var.get())
        except ValueError:
            messagebox.showwarning(title="警告", message="参数2输入不正确。")
            return

        if _param2 == 'T':
            value2 += 273.15

        for i, param in enumerate(self.properties.keys()):
            try:
                v = CoolProp.PropsSI(param, _param1, value1, _param2, value2, _create_mixture_string())
                if param == 'P':
                    v = v / 1e6
                elif param == 'T':
                    v -= 273.15
            except ValueError:
                v = None
            item_id = self.res_tree.get_children()[i]
            self.res_tree.set(item_id, 'value', v)