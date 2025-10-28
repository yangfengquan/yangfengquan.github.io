import tkinter as tk
from PipeAnalysisApp import PipeAnalysisApp
def main():
    """主函数"""
    root = tk.Tk()
    app = PipeAnalysisApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()