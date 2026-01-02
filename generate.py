import os
import subprocess
import sys

# Configuration
TARGET_DIRS = ['Src', 'Inc']
EXTENSIONS = ('.c', '.h', '.cpp', '.hpp')

def run_stm32pio_generate():
    """Runs the stm32pio generate command."""
    print("--- Generating Code with stm32pio ---")
    try:
        # Check if stm32pio is installed
        subprocess.run(['stm32pio', '--version'], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Run the generation command
        # This reads settings from stm32pio.ini automatically
        subprocess.run(['stm32pio', 'generate'], check=True)
        print("Code generation successful.\n")
        return True
    except FileNotFoundError:
        print("Error: 'stm32pio' command not found. Is it installed and in your PATH?")
        return False
    except subprocess.CalledProcessError as e:
        print(f"Error: stm32pio generation failed with exit code {e.returncode}.")
        return False

def run_clang_format():
    """Runs clang-format on all source files in TARGET_DIRS."""
    print("--- Formatting Code ---")
    
    project_root = os.path.dirname(os.path.abspath(__file__))
    formatted_count = 0
    
    # Check if clang-format exists
    try:
        subprocess.run(['clang-format', '--version'], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except FileNotFoundError:
        print("Error: 'clang-format' not found. Skipping formatting.")
        return

    for d in TARGET_DIRS:
        full_path = os.path.join(project_root, d)
        
        if not os.path.exists(full_path):
            print(f"Warning: Directory '{d}' not found. Skipping.")
            continue

        for root, _, files in os.walk(full_path):
            for file in files:
                if file.endswith(EXTENSIONS):
                    file_path = os.path.join(root, file)
                    try:
                        # -i edits in place, -style=file reads your .clang-format
                        subprocess.run(['clang-format', '-i', '-style=file', file_path], check=True)
                        formatted_count += 1
                        # Optional: Print progress
                        # print(f"Formatted: {file}")
                    except subprocess.CalledProcessError:
                        print(f"Failed to format {file}")

    print(f"Formatting complete. Processed {formatted_count} files.")

def main():
    # 1. Generate the code
    if not run_stm32pio_generate():
        sys.exit(1)

    # 2. Format the code
    run_clang_format()
    
    print("\n=== All Done ===")

if __name__ == "__main__":
    main()
