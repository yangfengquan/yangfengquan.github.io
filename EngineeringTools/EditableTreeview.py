import tkinter as tk
from tkinter import ttk

class EditableTreeview(ttk.Treeview):
    def __init__(self, master=None, editable_columns=None, **kwargs):
        """
        初始化可编辑的Treeview
        
        Args:
            master: 父组件
            editable_columns: 可编辑的列标识符列表，例如 ['#0', 'col1'] 或索引列表 [0, 1]
            **kwargs: 传递给ttk.Treeview的其他参数
        """
        super().__init__(master, **kwargs)
        
        # 设置可编辑列
        self.editable_columns = editable_columns if editable_columns is not None else []
        
        # 绑定双击事件
        self.bind('<Double-1>', self._on_double_click)
        
        # 用于存储当前编辑的组件
        self._editor = None
        self._editor_column = None
        self._editor_item = None

    def _on_double_click(self, event):
        """处理双击事件"""
        # 清除现有的编辑器
        self._destroy_editor()
        
        # 确定点击的区域
        region = self.identify_region(event.x, event.y)
        if region not in ['tree', 'cell']:
            return
        
        # 获取点击的列和项目
        column = self.identify_column(event.x)
        item = self.identify_row(event.y)
        
        if not item:
            return
        
        # 检查列是否可编辑
        column_id = self._get_column_id(column)
        if column_id not in self.editable_columns and column not in self.editable_columns:
            return
        
        # 获取单元格的值
        if column == '#0':
            value = self.item(item, 'text')
        else:
            col_index = int(column[1:]) - 1
            values = self.item(item, 'values')
            value = values[col_index] if col_index < len(values) else ''
        
        # 获取单元格的边界框
        x, y, width, height = self.bbox(item, column)
        
        # 创建编辑器
        self._create_editor(x, y, width, height, value, item, column_id)

    def _get_column_id(self, column):
        """将列索引转换为列标识符"""
        if column == '#0':
            return '#0'
        
        # 获取所有列标识符
        columns = self['columns']
        if columns:
            col_index = int(column[1:]) - 1
            if col_index < len(columns):
                return columns[col_index]
        
        return column

    def _create_editor(self, x, y, width, height, value, item, column):
        """创建编辑器组件"""
        # 创建Entry组件
        self._editor = ttk.Entry(self, width=width)
        self._editor.insert(0, value)
        self._editor.select_range(0, tk.END)
        self._editor.focus()
        
        # 放置编辑器
        self._editor.place(x=x, y=y, width=width, height=height)
        
        # 绑定事件
        self._editor.bind('<Return>', self._on_editor_return)
        self._editor.bind('<Escape>', self._on_editor_escape)
        self._editor.bind('<FocusOut>', self._on_editor_focus_out)
        
        # 保存编辑器信息
        self._editor_column = column
        self._editor_item = item

    def _destroy_editor(self, event=None):
        """销毁编辑器"""
        if self._editor:
            self._editor.destroy()
            self._editor = None
            self._editor_column = None
            self._editor_item = None

    def _on_editor_return(self, event):
        """处理编辑器回车事件"""
        self._save_editor_value()
        return 'break'  # 防止默认行为

    def _on_editor_escape(self, event):
        """处理编辑器ESC键事件"""
        self._destroy_editor()

    def _on_editor_focus_out(self, event):
        """处理编辑器失去焦点事件"""
        self._save_editor_value()

    def _save_editor_value(self):
        """保存编辑器的值"""
        if not self._editor:
            return
        
        # 获取编辑器的值
        new_value = self._editor.get()
        
        # 更新Treeview中的值
        if self._editor_column == '#0':
            self.item(self._editor_item, text=new_value)
        else:
            # 获取当前值
            current_values = list(self.item(self._editor_item, 'values'))
            
            # 确保有足够的列
            columns = self['columns']
            if columns and self._editor_column in columns:
                col_index = columns.index(self._editor_column)
                while len(current_values) <= col_index:
                    current_values.append('')
                current_values[col_index] = new_value
                
                # 更新值
                self.item(self._editor_item, values=current_values)
        
        # 销毁编辑器
        self._destroy_editor()
        
        # 触发事件
        self.event_generate('<<TreeviewCellEdited>>')