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
        root.title("Fair Market Auto")

        root.geometry("1000x650")

        # Overall Style
        root.configure(bg="#e7f0fb")  

        style = ttk.Style()
        try:
            style.theme_use("clam")
        except tk.TclError:
            pass

        style.configure("Main.TFrame", background="#e7f0fb")
        style.configure("Card.TFrame", background="white", relief="flat")
        style.configure("TLabel", background="#e7f0fb")
        style.configure("Title.TLabel", font=("Segoe UI", 22, "bold"), foreground="#12365b", background="#e7f0fb")
        style.configure("Subtitle.TLabel", font=("Segoe UI", 10), foreground="#345678", background="#e7f0fb")
        style.configure("Section.TLabel", font=("Segoe UI", 11, "bold"), foreground="#12365b", background="#e7f0fb")
        style.configure("Search.TButton", font=("Segoe UI", 11, "bold"), foreground="white", background="#1f6feb")
        style.map("Search.TButton", background=[("active", "#1553b0")])

        # Main Layout
        main_frame = ttk.Frame(root, style="Main.TFrame", padding="20 20 20 20")
        main_frame.grid(row=0, column=0, sticky="nsew")
        root.rowconfigure(0, weight=1)
        root.columnconfigure(0, weight=1)

        main_frame.columnconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=2)

        # Header
        title = ttk.Label(main_frame, text="Fair Market Auto", style="Title.TLabel")
        title.grid(row=0, column=0, columnspan=2, sticky="w")

        subtitle = ttk.Label(
            main_frame,
            text="Compare data structures while helping students find fair car prices.",
            style="Subtitle.TLabel"
        )
        subtitle.grid(row=1, column=0, columnspan=2, sticky="w", pady=(4, 16))

        # Left card: controls 
        left_card = ttk.Frame(main_frame, style="Card.TFrame", padding="18 16 18 18")
        left_card.grid(row=2, column=0, sticky="nsew", padx=(0, 12))
        main_frame.rowconfigure(2, weight=1)

        # Data structure buttons
        ds_label = ttk.Label(left_card, text="Data Structure", style="Section.TLabel")
        ds_label.grid(row=0, column=0, sticky="w")

        self.structure_var = tk.StringVar(value="hashtable")
        ds_frame = ttk.Frame(left_card, style="Card.TFrame")
        ds_frame.grid(row=1, column=0, sticky="w", pady=(4, 10))
        ttk.Radiobutton(ds_frame, text="Hash Table", variable=self.structure_var, value="hashtable").pack(side=tk.LEFT)
        ttk.Radiobutton(ds_frame, text="Tree Map", variable=self.structure_var, value="treemap").pack(side=tk.LEFT, padx=10)

        divider = ttk.Separator(left_card, orient="horizontal")
        divider.grid(row=2, column=0, sticky="ew", pady=(8, 10))

        # Search criteria
        criteria_label = ttk.Label(left_card, text="Search Criteria", style="Section.TLabel")
        criteria_label.grid(row=3, column=0, sticky="w", pady=(0, 6))

        form_frame = ttk.Frame(left_card, style="Card.TFrame")
        form_frame.grid(row=4, column=0, sticky="ew")
        form_frame.columnconfigure(1, weight=1)

        # Brand
        ttk.Label(form_frame, text="Brand:").grid(row=0, column=0, sticky="w", pady=3)
        self.brand_entry = ttk.Entry(form_frame, width=24)
        self.brand_entry.grid(row=0, column=1, sticky="ew", pady=3)

        # Model
        ttk.Label(form_frame, text="Model:").grid(row=1, column=0, sticky="w", pady=3)
        self.model_entry = ttk.Entry(form_frame, width=24)
        self.model_entry.grid(row=1, column=1, sticky="ew", pady=3)

        # Year
        ttk.Label(form_frame, text="Year:").grid(row=2, column=0, sticky="w", pady=3)
        self.year_entry = ttk.Entry(form_frame, width=24)
        self.year_entry.grid(row=2, column=1, sticky="ew", pady=3)

        # Asking price
        ttk.Label(form_frame, text="Asking Price (optional):").grid(row=3, column=0, sticky="w", pady=3)
        self.ask_price_entry = ttk.Entry(form_frame, width=24)
        self.ask_price_entry.grid(row=3, column=1, sticky="ew", pady=3)

        # Search button
        search_btn = ttk.Button(left_card, text="Search", style="Search.TButton", command=self.search)
        search_btn.grid(row=5, column=0, pady=(14, 0), sticky="ew")

        # Right card: results 
        right_card = ttk.Frame(main_frame, style="Card.TFrame", padding="16 14 16 16")
        right_card.grid(row=2, column=1, sticky="nsew")
        main_frame.rowconfigure(2, weight=1)
        right_card.rowconfigure(1, weight=1)
        right_card.columnconfigure(0, weight=1)

        results_label = ttk.Label(right_card, text="Results", style="Section.TLabel")
        results_label.grid(row=0, column=0, sticky="w", pady=(0, 6))

        self.results_text = scrolledtext.ScrolledText(
            right_card,
            width=85,
            height=26,
            font=("Consolas", 9),
            bg="#f7f9ff",
            fg="#222222",
            borderwidth=0,
        )
        self.results_text.grid(row=1, column=0, sticky="nsew")

        self.results_text.tag_config("header", foreground="#1f4e79", font=("Consolas", 10, "bold"))
        self.results_text.tag_config("section", font=("Consolas", 9, "bold"))
        self.results_text.tag_config("divider", foreground="#1f4e79")

    # search + formatting 

    def search(self):
        brand = self.brand_entry.get().strip()
        model = self.model_entry.get().strip()
        year = self.year_entry.get().strip()
        structure = self.structure_var.get()

        ask_price_str = self.ask_price_entry.get().strip()
        asking_price = None
        if ask_price_str:
            try:
                asking_price = float(ask_price_str)
            except ValueError:
                asking_price = None

        if not brand or not model or not year:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, "Please fill in Brand, Model, and Year.")
            return

        # need to adjust path: 
        #issue - cannot resolve file location refer to main.cpp for issue with main.exe creation
        exe_path = "src/main.exe"

        if not os.path.exists(exe_path):
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(
                tk.END,
                f"Error: Executable not found at {exe_path}\n"
                f"Please compile main.cpp first."
            )
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
                self.format_output(output, structure, asking_price)
            else:
                self.results_text.insert(
                    tk.END,
                    f"Error: exception B reached\n{result.stderr}"
                )

        except subprocess.TimeoutExpired:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, "Search timed out.")
        except Exception as e:
            self.results_text.delete(1.0, tk.END)
            self.results_text.insert(tk.END, f"Error: exception A reached {str(e)}")

    def format_output(self, output, structure, asking_price):
        lines = output.strip().split("\n")

        stats = {}
        cars = []
        in_cars_section = False

        for line in lines:
            if not in_cars_section and ":" in line and not line.startswith("---"):
                key, value = line.split(":", 1)
                stats[key] = value.strip()
            elif line.strip() == "---CARS---":
                in_cars_section = True
            elif in_cars_section:
                cars.append(line)

        self.results_text.insert(tk.END, f"{'=' * 80}\n", "divider")
        self.results_text.insert(tk.END, "  SEARCH RESULTS\n", "header")
        self.results_text.insert(tk.END, f"{'=' * 80}\n\n", "divider")

        self.results_text.insert(tk.END, "Search Summary:\n", "section")
        self.results_text.insert(tk.END, f"  Data structure: {structure}\n")
        self.results_text.insert(tk.END, f"  Time: {stats.get('TIME_MS', 'N/A')} ms\n")
        self.results_text.insert(tk.END, f"  Matches: {stats.get('COUNT', 'N/A')}\n\n")

        count_str = stats.get("COUNT", "0")

        if count_str != "0":
            # Price stats
            self.results_text.insert(tk.END, "Price Statistics:\n", "section")
            self.results_text.insert(tk.END, f"  Lowest:  ${stats.get('LOWEST', 'N/A')}\n")
            self.results_text.insert(tk.END, f"  Highest: ${stats.get('HIGHEST', 'N/A')}\n")
            self.results_text.insert(tk.END, f"  Median:  ${stats.get('MEDIAN', 'N/A')}\n")
            self.results_text.insert(tk.END, f"  Average: ${stats.get('AVERAGE', 'N/A')}\n\n")

            # Deal evaluation
            if asking_price is not None:
                avg_str = stats.get("AVERAGE")
                try:
                    avg_val = float(avg_str)
                    diff = asking_price - avg_val
                    pct = (diff / avg_val) * 100 if avg_val else 0

                    self.results_text.insert(tk.END, "Deal Evaluation:\n", "section")
                    self.results_text.insert(tk.END, f"  Your price: ${asking_price:.2f}\n")
                    self.results_text.insert(tk.END, f"  Difference from average: {diff:+.0f} ({pct:+.1f}%)\n")

                    if diff < 0:
                        msg = "  This looks like a good deal."
                    elif abs(diff) <= avg_val * 0.05:
                        msg = "  This looks like a fair deal."
                    else:
                        msg = "  This looks a bit expensive."

                    self.results_text.insert(tk.END, f"{msg}\n\n")
                except:
                    self.results_text.insert(tk.END, "Deal Evaluation:\n  (Average unavailable.)\n\n")

            # Cars
            self.results_text.insert(tk.END, f"{'=' * 80}\n", "divider")
            self.results_text.insert(tk.END, "Matching Vehicles:\n\n", "section")

            for car in cars:
                self.results_text.insert(tk.END, f"  {car}\n")
        else:
            self.results_text.insert(tk.END, "\nNo matching vehicles found.\n")


if __name__ == "__main__":
    root = tk.Tk()
    app = CarSearchGUI(root)
    root.mainloop()
