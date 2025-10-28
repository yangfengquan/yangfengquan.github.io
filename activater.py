import tkinter as tk
from tkinter import messagebox, ttk
import hashlib

class ActivationCodeGenerator:
    """激活码生成工具类"""
    def __init__(self, root):
        self.root = root
        self.root.title("激活码生成工具")
        self.root.geometry("500x400")
        self.root.resizable(False, False)
        
        # 配置样式
        self.style = ttk.Style()
        self.style.configure("TButton", font=("Arial", 10))
        self.style.configure("TLabel", font=("Arial", 10))
        self.style.configure("Header.TLabel", font=("Arial", 14, "bold"))
        
        # 与主程序相同的密钥，必须保持一致
        #self.secret_key = "Ypipe2025"
        #self.secret_key = "YTools2025"
        
        self.create_widgets()
    
    def create_widgets(self):
        """创建界面组件"""
        # 标题
        ttk.Label(self.root, text="激活码生成工具", style="Header.TLabel").pack(pady=20)
        
        # 主框架
        main_frame = ttk.Frame(self.root, padding="20")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 申请码输入区域
        ttk.Label(main_frame, text="请输入申请码:").pack(anchor=tk.W, pady=(10, 5))
        self.request_code_var = tk.StringVar()
        request_entry = ttk.Entry(main_frame, textvariable=self.request_code_var, width=50)
        request_entry.pack(fill=tk.X, pady=(0, 10))

        ttk.Label(main_frame, text="请输入密钥:").pack(anchor=tk.W, pady=(10, 5))
        self.secret_key_var = tk.StringVar()
        request_entry = ttk.Entry(main_frame, textvariable=self.secret_key_var, width=50)
        request_entry.pack(fill=tk.X, pady=(0, 10))
        
        # 生成按钮
        ttk.Button(main_frame, text="生成激活码", command=self.generate_activation_code).pack(pady=15)
        
        # 激活码显示区域
        ttk.Label(main_frame, text="生成的激活码:").pack(anchor=tk.W, pady=(10, 5))
        
        self.activation_code_var = tk.StringVar()
        activation_entry = ttk.Entry(main_frame, textvariable=self.activation_code_var, 
                                    state="readonly", width=50)
        activation_entry.pack(fill=tk.X, pady=(0, 10))
        
        # 复制按钮
        ttk.Button(main_frame, text="复制激活码", 
                  command=lambda: self.root.clipboard_append(self.activation_code_var.get())).pack(pady=5)
        
        # 状态标签
        self.status_var = tk.StringVar(value="请输入申请码并点击生成按钮")
        ttk.Label(main_frame, textvariable=self.status_var, foreground="blue").pack(side=tk.BOTTOM, pady=10)
    
    def generate_activation_code(self):
        """根据申请码生成激活码"""
        request_code = self.request_code_var.get().strip()
        secret_key = self.secret_key_var.get().strip()
        if not request_code:
            messagebox.showerror("错误", "请输入申请码")
            return
            
        # 验证申请码格式 (8-8-8-8格式)
        parts = request_code.split('-')
        if len(parts) != 4 or any(len(part) != 8 for part in parts):
            messagebox.showerror("错误", "申请码格式不正确\n正确格式应为: XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX")
            return
        
        try:
            # 使用与主程序相同的算法生成激活码
            combined = f"{request_code}-{secret_key}"
            hash_obj = hashlib.sha512(combined.encode('utf-8'))
            hash_hex = hash_obj.hexdigest()
            activation_code = '-'.join([hash_hex[i:i+8] for i in range(0, 48, 8)]).upper()
            
            self.activation_code_var.set(activation_code)
            self.status_var.set("激活码生成成功，可以复制使用")
        except Exception as e:
            messagebox.showerror("错误", f"生成激活码失败: {str(e)}")
            self.status_var.set("生成失败，请重试")


if __name__ == "__main__":
    root = tk.Tk()
    app = ActivationCodeGenerator(root)
    root.mainloop()
    