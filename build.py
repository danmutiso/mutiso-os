import os
import pathlib
import subprocess
from getpass import getuser
import shutil

# parent directory
os_dir = os.getcwd()
os_build_dir = "build/"
os_iso_dir = "build/isodir"
c_cross_compiler = f"/home/{getuser()}/opt/cross/bin/i686-elf-gcc"
ld_cross_compiler = f"/home/{getuser()}/opt/cross/bin/i686-elf-ld"
object_file_suffix = ".o"
iso_image_name = "mutiso-os.iso"

def build_src_files(os_directory):
    try:
        for root, dir, files in os.walk(os_directory):
            for file in files:
                filepath = root + "/" + file
                filename = pathlib.Path(filepath)
                if filename.suffix == ".asm":
                    asm_builder(filepath, os_build_dir, filename.name.strip(filename.suffix), object_file_suffix)
                    print(f"[asm] nasm -f elf32 {filepath} -o {os_build_dir}{filename.name.strip(filename.suffix)}{object_file_suffix}")
                elif filename.suffix == ".c":
                    c_builder(filepath, os_build_dir, filename.name.strip(filename.suffix), object_file_suffix)
                    print(f"[gcc] {c_cross_compiler} -ffreestanding -fno-stack-protector -c {filepath} -o {os_build_dir}{filename.name.strip(filename.suffix)}{object_file_suffix}")
                else:
                    continue
    except Exception as os_build_Error:
        print(os_build_Error)

def asm_builder(source_path, build_path, filename, suffix):
    try:
        subprocess.run(["nasm", "-f", "elf32", f"{source_path}", "-o", f"{build_path}{filename}{suffix}"])
    except Exception as asm_builder_Error:
        print(asm_builder_Error)

def c_builder(source_path, build_path, filename, suffix):
    try:
        subprocess.run([f"{c_cross_compiler}", "-ffreestanding", "-fno-stack-protector", "-c", f"{source_path}", "-o", f"{build_path}{filename}{suffix}"])
    except Exception as asm_builder_Error:
        print(asm_builder_Error)

def link_object_files():
    try:
        object_file_list = []
        command = f"{ld_cross_compiler} -T linker.ld -o {os_build_dir}kernel.elf "

        for root, dir, files in os.walk(os_build_dir):
            for file in files:
                filepath = root + file
                filename = pathlib.Path(filepath)
                if filename.suffix == ".o":
                    object_file_list.append(filepath)
                    print(f"[linker] appending object file {filename} to object file list")

        for i in range(len(object_file_list)):
            command += object_file_list[i] + " "

        print(f"[linker] linking all object files. This might take some time ......")
        subprocess.run(command, shell=True)
    except Exception as linker_Error:
        print(linker_Error)

def build_iso():
    try:
        shutil.copy(f"{os_build_dir}kernel.elf", f"{os_iso_dir}/boot/")
        shutil.copy(f"{os_dir}/grub/grub.cfg", f"{os_iso_dir}/boot/grub/")
        subprocess.run(["grub-mkrescue", "-o", f"{os_build_dir}{iso_image_name}", f"{os_iso_dir}"])
        print(f"[iso-build] iso image build output at {os_build_dir} name {iso_image_name} . SUCCESS")
    except Exception as iso_build_Error:
        print(iso_build_Error)

def run_in_qemu():
    try:
        print(f"[qemu-exec] running {os_build_dir}{iso_image_name}")
        subprocess.run(["qemu-system-i386", "-cdrom", f"{os_build_dir}{iso_image_name}"])
    except Exception as qemu_execution_Error:
        print(qemu_execution_Error)

build_src_files(os_dir)
link_object_files()
build_iso()
run_in_qemu()
