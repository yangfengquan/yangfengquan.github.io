import tkinter as tk
from tkinter import messagebox, ttk, StringVar
import hashlib
import platform
import subprocess
import uuid
import os
import json
import winreg  # 仅Windows系统可用

# 确保程序只在Windows上运行
if platform.system() != "Windows":
    messagebox.showerror("错误", "此程序仅支持Windows系统")
    exit(1)


class RegistryStorage:
    """使用Windows注册表存储数据的类（不加密）"""
    def __init__(self, app_name="MyApplication"):
        self.app_name = app_name
        # 注册表路径
        self.registry_path = f"Software\\{app_name}\\Settings"
        # 注册表项名称
        self.trial_count_key = "TrialCount"
        self.activation_key = "ActivationData"
        
    def _get_registry_key(self, access=winreg.KEY_READ):
        """获取或创建注册表项"""
        try:
            # 尝试打开现有项
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, self.registry_path, 0, access)
        except FileNotFoundError:
            # 不存在则创建
            key = winreg.CreateKey(winreg.HKEY_CURRENT_USER, self.registry_path)
        return key
    
    def get_trial_count(self):
        """从注册表获取试用次数（不加密）"""
        try:
            key = self._get_registry_key()
            # 直接读取整数形式的试用次数
            trial_count, _ = winreg.QueryValueEx(key, self.trial_count_key)
            winreg.CloseKey(key)
            return int(trial_count)
        except:
            return 0
    
    def update_trial_count(self, count):
        """更新注册表中的试用次数（不加密）"""
        try:
            # 直接存储整数形式的试用次数
            key = self._get_registry_key(winreg.KEY_WRITE)
            winreg.SetValueEx(key, self.trial_count_key, 0, winreg.REG_DWORD, count)
            winreg.CloseKey(key)
            return True
        except Exception as e:
            print(f"更新注册表失败: {e}")
            return False
    
    def get_activation_data(self):
        """从注册表获取激活数据"""
        try:
            key = self._get_registry_key()
            activation_data, _ = winreg.QueryValueEx(key, self.activation_key)
            winreg.CloseKey(key)
            return json.loads(activation_data)
        except:
            return None
    
    def save_activation_data(self, data):
        """保存激活数据到注册表"""
        try:
            # 序列化数据
            activation_data = json.dumps(data)
            
            # 写入注册表
            key = self._get_registry_key(winreg.KEY_WRITE)
            winreg.SetValueEx(key, self.activation_key, 0, winreg.REG_SZ, activation_data)
            winreg.CloseKey(key)
            return True
        except Exception as e:
            print(f"保存激活数据到注册表失败: {e}")
            return False
    
    def delete_registry_data(self):
        """删除注册表中的数据（用于测试）"""
        try:
            key = self._get_registry_key(winreg.KEY_WRITE)
            winreg.DeleteValue(key, self.trial_count_key)
            winreg.DeleteValue(key, self.activation_key)
            winreg.CloseKey(key)
            return True
        except:
            return False


class HardwareActivation:
    """硬件激活码生成与验证类"""
    def __init__(self, secret_key="MySecretKey123"):
        self.secret_key = secret_key
        self.hardware_info = None
        self.request_code = None
        
    def get_hardware_info(self):
        """获取硬件信息"""
        info = []
        
        # 操作系统信息
        info.append(f"OS: {platform.system()} {platform.release()}")
        
        # 主机名
        info.append(f"Hostname: {platform.node()}")
        
        # Windows CPU信息
        try:
            cpu_info = subprocess.check_output(
                ["wmic", "cpu", "get", "Name"],
                universal_newlines=True,
                stderr=subprocess.STDOUT
            ).strip().split("\n")[1]
            info.append(f"CPU: {cpu_info}")
            
            # 主板序列号
            board_sn = subprocess.check_output(
                ["wmic", "baseboard", "get", "SerialNumber"],
                universal_newlines=True,
                stderr=subprocess.STDOUT
            ).strip().split("\n")[1]
            info.append(f"Motherboard SN: {board_sn}")
        except Exception as e:
            info.append(f"HW Error: {str(e)}")
        
        # MAC地址
        try:
            mac = ':'.join(['{:02x}'.format((uuid.getnode() >> elements) & 0xff) 
                           for elements in range(0,2*6,2)][::-1])
            info.append(f"MAC Address: {mac}")
        except:
            pass
        
        self.hardware_info = info
        return info
    
    def generate_request_code(self):
        """生成申请码"""
        if not self.hardware_info:
            self.get_hardware_info()
            
        hardware_str = "|".join(self.hardware_info)
        hash_obj = hashlib.sha256(hardware_str.encode('utf-8'))
        hash_hex = hash_obj.hexdigest()
        self.request_code = '-'.join([hash_hex[i:i+8] for i in range(0, 32, 8)]).upper()
        return self.request_code
    
    def generate_activation_code(self, request_code=None):
        """生成激活码"""
        used_request_code = request_code if request_code else self.request_code
        if not used_request_code:
            self.generate_request_code()
            used_request_code = self.request_code
            
        combined = f"{used_request_code}-{self.secret_key}"
        hash_obj = hashlib.sha512(combined.encode('utf-8'))
        hash_hex = hash_obj.hexdigest()
        return '-'.join([hash_hex[i:i+8] for i in range(0, 48, 8)]).upper()
    
    def verify_activation(self, activation_code, request_code=None):
        """验证激活码"""
        generated_code = self.generate_activation_code(request_code)
        return generated_code == activation_code