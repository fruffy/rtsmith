import subprocess
import os

def generate_config(seed, p4_program):
    command = f"backends/p4tools/modules/p4rtsmith/p4rtsmith --target bmv2 --arch v1model --seed {seed} {p4_program}"
    subprocess.run(command, shell=True)

def run_test(p4_program, config_file):
    command = f"sudo -E ../backends/p4tools/modules/p4rtsmith/targets/bmv2/test/run-bmv2-proto-test.py .. {p4_program} -tf {config_file}"
    subprocess.run(command, shell=True)

def find_p4c_build_dir():
    current_dir = os.path.abspath(__file__)
    
    while True:
        p4c_build_dir = os.path.join(current_dir, "p4c", "build")
        if os.path.exists(p4c_build_dir):
            return p4c_build_dir
        
        current_dir = os.path.dirname(current_dir)
        
        if current_dir == os.path.dirname(current_dir):
            raise RuntimeError("p4c/build directory not found.")

def main():
    p4c_build_dir = find_p4c_build_dir()
    os.chdir(p4c_build_dir)

    seed = 1
    p4_program_name = "v1model_dead_ternary_table"
    config_file = os.path.join(p4c_build_dir, f"gen_configs/initial_config.txtpb")

    p4_program_path = f"../backends/p4tools/modules/flay/targets/bmv2/test/programs/{p4_program_name}.p4"

    generate_config(seed, p4_program_path)
    run_test(p4_program_path, config_file)

if __name__ == "__main__":
    main()