import os
import tkinter as tk
from tkinter import ttk, scrolledtext

# Optional imports. These may fail if packages are not installed.
try:
    import openai
except ImportError:
    openai = None

try:
    import google.generativeai as genai
except ImportError:
    genai = None

try:
    import anthropic
except ImportError:
    anthropic = None


class AIPromptApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("AI Prompt App")
        self.geometry("600x400")
        self.create_widgets()
        self.model_var.set('openai')

    def create_widgets(self):
        left_frame = ttk.Frame(self)
        left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)

        self.model_var = tk.StringVar()
        models = [
            ('OpenAI', 'openai'),
            ('Gemini', 'gemini'),
            ('Claude', 'claude'),
        ]
        for text, value in models:
            rb = ttk.Radiobutton(left_frame, text=text, variable=self.model_var, value=value)
            rb.pack(anchor=tk.W, pady=2)

        right_frame = ttk.Frame(self)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=5, pady=5)

        self.prompt_entry = scrolledtext.ScrolledText(right_frame, height=10)
        self.prompt_entry.pack(fill=tk.BOTH, expand=True)

        send_button = ttk.Button(right_frame, text="Send", command=self.send_prompt)
        send_button.pack(pady=5)

        self.response_text = scrolledtext.ScrolledText(right_frame, height=10)
        self.response_text.pack(fill=tk.BOTH, expand=True)

    def send_prompt(self):
        model = self.model_var.get()
        prompt = self.prompt_entry.get('1.0', tk.END).strip()
        if not prompt:
            return

        if model == 'openai' and openai:
            key = os.getenv('OPENAI_API_KEY')
            if not key:
                self.response_text.insert(tk.END, 'Missing OPENAI_API_KEY\n')
                return
            openai.api_key = key
            resp = openai.ChatCompletion.create(model='gpt-3.5-turbo', messages=[{'role':'user','content':prompt}])
            answer = resp.choices[0].message.content
        elif model == 'gemini' and genai:
            key = os.getenv('GOOGLE_API_KEY')
            if not key:
                self.response_text.insert(tk.END, 'Missing GOOGLE_API_KEY\n')
                return
            genai.configure(api_key=key)
            model = genai.GenerativeModel('gemini-pro')
            answer = model.generate_content(prompt).text
        elif model == 'claude' and anthropic:
            key = os.getenv('ANTHROPIC_API_KEY')
            if not key:
                self.response_text.insert(tk.END, 'Missing ANTHROPIC_API_KEY\n')
                return
            client = anthropic.Client(api_key=key)
            resp = client.completions.create(model='claude-instant-1', prompt=prompt, max_tokens_to_sample=200)
            answer = resp.completion
        else:
            answer = f'Model {model} not available or required package not installed.'

        self.response_text.delete('1.0', tk.END)
        self.response_text.insert(tk.END, answer + '\n')


if __name__ == '__main__':
    app = AIPromptApp()
    app.mainloop()
