import os

Import("env")

# Enable the option to include toolchain paths
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

# Optional: Override the path if you want it in the root instead of .pio/build/...
# env.Replace(COMPILATIONDB_PATH=os.path.join("$PROJECT_DIR", "compile_commands.json"))
