# AI Prompt App

This is a simple Python GUI application demonstrating how to send prompts to different AI models. It uses Tkinter for the UI and supports OpenAI, Google Gemini, and Anthropic Claude models.

## Installation

1. Install Python 3.8 or newer on Windows.
2. Install the required packages:

```bash
pip install openai google-generativeai anthropic
```

Set your API keys in environment variables:

- `OPENAI_API_KEY` for OpenAI
- `GOOGLE_API_KEY` for Gemini
- `ANTHROPIC_API_KEY` for Claude

## Running the App

Run the script with Python:

```bash
python app.py
```

A window will open with a list of models on the left and a prompt entry on the right. Select a model, enter your prompt, and press `Send` to receive a response.
