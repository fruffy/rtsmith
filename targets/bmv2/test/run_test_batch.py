import subprocess
import os
from pathlib import Path

def generate_config(p4rtsmith_path, seed, p4_program_path, output_dir, config_file_path):
    command = f"{p4rtsmith_path} --target bmv2 --arch v1model --seed {seed} --output-dir {output_dir} --generate-config {config_file_path} {p4_program_path}"
    subprocess.run(command, shell=True)

def run_test(run_test_script, p4_program_path, config_file_path):
    command = f"sudo -E {run_test_script} .. {p4_program_path} -tf {config_file_path}"
    subprocess.run(command, shell=True)

def find_p4c_dir():
    current_dir = Path(__file__).resolve()

    while True:
        p4c_build_dir = current_dir / "p4c"
        if p4c_build_dir.exists():
            return p4c_build_dir

        current_dir = current_dir.parent

        if current_dir == current_dir.parent:
            raise RuntimeError("p4c/build directory not found.")
        

def main():
    p4c_dir = find_p4c_dir()
    p4c_build_dir = p4c_dir / "build"
    os.chdir(p4c_build_dir)

    seed = 1
    output_dir = p4c_build_dir / "gen_configs"
    config_file_path = "initial_config.txtpb"
    p4rtsmith_path = p4c_build_dir / "backends" / "p4tools" / "modules" / "p4rtsmith" / "p4rtsmith"
    run_test_script = p4c_dir / "backends" / "p4tools" / "modules" / "p4rtsmith" / "targets" / "bmv2" / "test" / "run-bmv2-proto-test.py"
    p4_program_path = p4c_dir / "backends" / "p4tools" / "modules" / "flay" / "targets" / "bmv2" / "test" / "programs" / "v1model_dead_ternary_table.p4"

    generate_config(p4rtsmith_path, seed, p4_program_path, output_dir, config_file_path)
    run_test(run_test_script, p4_program_path, output_dir / config_file_path)


if __name__ == "__main__":
    main()