import shutil
import os

if input("This will delete mouse_base_export. Proceed? (Y/N): ").lower() != "y":
    exit(-1)

shutil.rmtree("../mouse_base_export")

shutil.copytree("src", "../mouse_base_export")

os.rename("../mouse_base_export/main.cpp", "../mouse_base_export/mouse_base_export.ino")
