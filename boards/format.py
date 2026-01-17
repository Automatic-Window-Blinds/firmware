import os
import subprocess

# 1. Target the directory where this script is located (".")
TARGET_DIRS = ["."]

# 2. Extensions to format
EXTENSIONS = (".c", ".h", ".cpp", ".hpp")

# 3. Exclude 'Drivers' (Standard CubeMX HAL) and hidden folders
SKIP_DIRS = {"Drivers", ".git", ".vscode", "build", ".pio"}


def run_clang_format():
    """Runs clang-format on all source files in TARGET_DIRS recursively."""
    print("--- Formatting Code ---")

    # Get the absolute path of the directory containing this script (e.g., .../boards)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    formatted_count = 0

    # Check if clang-format exists
    try:
        subprocess.run(
            ["clang-format", "--version"],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except FileNotFoundError:
        print("Error: 'clang-format' not found. Please install it first.")
        return

    for d in TARGET_DIRS:
        # Resolve path relative to the script location
        full_path = os.path.join(script_dir, d)

        if not os.path.exists(full_path):
            print(f"Warning: Directory '{d}' not found. Skipping.")
            continue

        # os.walk is recursive
        for root, dirs, files in os.walk(full_path):
            # Prune directories we don't want to enter
            # modifying 'dirs' in-place tells os.walk to skip them
            dirs[:] = [d for d in dirs if d not in SKIP_DIRS]

            for file in files:
                if file.endswith(EXTENSIONS):
                    file_path = os.path.join(root, file)
                    try:
                        # -i edits in place, -style=file reads your .clang-format
                        subprocess.run(
                            ["clang-format", "-i", "-style=file", file_path], check=True
                        )
                        formatted_count += 1

                        # Print path relative to the script directory for cleaner output
                        rel_path = os.path.relpath(file_path, script_dir)
                        print(f"Formatted: {rel_path}")

                    except subprocess.CalledProcessError:
                        print(f"Failed to format {file}")

    print(f"Formatting complete. Processed {formatted_count} files.")


def main():
    run_clang_format()


if __name__ == "__main__":
    main()
