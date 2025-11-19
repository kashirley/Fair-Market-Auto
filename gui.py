# -*- coding: utf-8 -*-
import tkinter as tk
from tkinter import ttk, scrolledtext
import subprocess
import os

# RESOLVED: UTF-8 char removed - Error: SyntaxError: Non-UTF-8 code starting with '\x95' in file 
#C:\Users\katie\source\COP3530\Project 3\Fair-Market-Auto\gui.py on line 140, 
#but no encoding declared; see https://peps.python.org/pep-0263/ for details
# Press any key to continue . . . 

class CarSearchGUI:
    def __init__(self, root):
        self.root = root
        root.title("Car Price Comparison Tool")
        root.geometry("700x600")
        
        # Main frame 
        main_frame = ttk.Frame(root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Header - 
        title = ttk.Label(main_frame, text="Fair Market Auto", 
                         font=('Arial', 18, 'bold'))
        title.grid(row=0, column=0, columnspan=2, pady=10)
        
        # Gui for data structure selection
        ttk.Label(main_frame, text="Data Structure:", 
                 font=('Arial', 10, 'bold')).grid(row=1, column=0, sticky=tk.W, pady=5)
        self.structure_var = tk.StringVar(value="hashtable")
        structure_frame = ttk.Frame(main_frame)
        structure_frame.grid(row=1, column=1, sticky=tk.W, pady=5)
        ttk.Radiobutton(structure_frame, text="HashTable", 
                       variable=self.structure_var, value="hashtable").pack(side=tk.LEFT)
        ttk.Radiobutton(structure_frame, text="TreeMap", 
                       variable=self.structure_var, value="treemap").pack(side=tk.LEFT, padx=10)
        
        # Search fields
        ttk.Label(main_frame, text="Brand:").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.brand_entry = ttk.Entry(main_frame, width=30)
        self.brand_entry.grid(row=2, column=1, sticky=tk.W, pady=5)
        
        ttk.Label(main_frame, text="Model:").grid(row=3, column=0, sticky=tk.W, pady=5)
        self.model_entry = ttk.Entry(main_frame, width=30)
        self.model_entry.grid(row=3, column=1, sticky=tk.W, pady=5)
        
        ttk.Label(main_frame, text="Year:").grid(row=4, column=0, sticky=tk.W, pady=5)
        self.year_entry = ttk.Entry(main_frame, width=30)
        self.year_entry.grid(row=4, column=1, sticky=tk.W, pady=5)
        
        # Search button
        search_btn = ttk.Button(main_frame, text="Search", command=self.search)
        search_btn.grid(row=5, column=0, columnspan=2, pady=15)
        
        
        ttk.Label(main_frame, text="Results:", 
                 font=('Arial', 10, 'bold')).grid(row=6, column=0, columnspan=2, sticky=tk.W)
        
        self.results_text = scrolledtext.ScrolledText(main_frame, width=80, height=20)
        self.results_text.grid(row=7, column=0, columnspan=2, pady=10)
        
    def search(self):
        brand = self.brand_entry.get().strip()
        model = self.model_entry.get().strip()
        year = self.year_entry.get().strip()
        structure = self.structure_var.get()
        
        if not brand or not model or not year:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, "Please fill in all fields.")
            return
        
        # need to adjust path: 
        #issue - cannot resolve file location refer to main.cpp for issue with main.exe creation
        exe_path = "src/main.exe" 
        
        if not os.path.exists(exe_path):
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, 
                f"Error: Executable not found at {exe_path}\n"
                f"Please compile main.cpp first.")
            return
        
        try:
            result = subprocess.run(
                [exe_path, structure, brand, model, year],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            self.results_text.delete(1.0, tk.END)
            
            if result.returncode == 0:
                output = result.stdout
                self.format_output(output)
            else:
                self.results_text.insert(tk.END, f"Error: exception B reached\n{result.stderr}") #Error reached - resolution: 
                
        except subprocess.TimeoutExpired:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, "Search timed out.")
        except Exception as e:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, f"Error: exception A reached {str(e)}")
    
    def format_output(self, output):
        lines = output.strip().split('\n')
        
        stats = {}
        cars = []
        in_cars_section = False
        
        for line in lines:
            if ':' in line and not in_cars_section:
                key, value = line.split(':', 1)
                stats[key] = value.strip()
            elif line == "---CARS---":
                in_cars_section = True
            elif in_cars_section:
                cars.append(line)
        
        self.results_text.insert(tk.END, f"{'='*70}\n", 'header')
        self.results_text.insert(tk.END, f"  SEARCH RESULTS\n", 'header')
        self.results_text.insert(tk.END, f"{'='*70}\n\n", 'header')
        
        self.results_text.insert(tk.END, f"Search Time: {stats.get('TIME_MS', 'N/A')} ms\n", 'bold')
        self.results_text.insert(tk.END, f"Vehicles Found: {stats.get('COUNT', 'N/A')}\n\n", 'bold')
        
        if stats.get('COUNT', '0') != '0':
            self.results_text.insert(tk.END, "Price Statistics:\n", 'bold')
            self.results_text.insert(tk.END, f"  Lowest:  ${stats.get('LOWEST', 'N/A')}\n")
            self.results_text.insert(tk.END, f"  Highest: ${stats.get('HIGHEST', 'N/A')}\n")
            self.results_text.insert(tk.END, f"  Average: ${stats.get('AVERAGE', 'N/A')}\n\n")
            
            self.results_text.insert(tk.END, f"{'='*70}\n", 'header')
            self.results_text.insert(tk.END, "Matching Vehicles:\n\n", 'bold')
            
            for car in cars:
                if car.strip():
                    self.results_text.insert(tk.END, f"  {car}\n")
        else:
            self.results_text.insert(tk.END, "\nNo matching vehicles found.\n")
        
        self.results_text.tag_config('header', foreground='blue')
        self.results_text.tag_config('bold', font=('Arial', 10, 'bold'))

if __name__ == "__main__":
    root = tk.Tk()
    app = CarSearchGUI(root)
    root.mainloop()
