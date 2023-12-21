# Usage: python scripts\python\compile_str.py sizeof(int) int
# Bugs: on Windows, if the program is crashed, it may cause the terminal input problem: if input UP key, shows "^[[A"

import argparse
import os
import subprocess  

file = "__c_pytyon__0a_.c"
compile_path = "gcc"

def generate_code(code_block, code_type):  
    if code_type == 'int':  
        return "#include <stdio.h>\n\nint main()\n{\n    printf(\"" + code_block + ": %d\\n\", " + code_block + ");\n    return 0;\n}"  
    elif code_type == 'str':  
        return "#include <stdio.h>\n\nint main()\n{\n    printf(\"" + code_block + ": %s\\n\", " + code_block + ");\n    return 0;\n}"  
    else:  
        return "Invalid code type. Please use 'int' or 'str'."  

def write_str_to_file(filename, str): 
	file = open(filename, 'w')   
	file.write(str)  
	file.close()

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='generate code by code block and code type')  
	parser.add_argument('param1', type=str, help='code block')  
	parser.add_argument('param2', type=str, help='code type')  
	parser.add_argument('param3', type=str, help='compiler type')  
	
	args = parser.parse_args()  
	
	print(f'param1: {args.param1}')  
	print(f'param2: {args.param2}')
	print(f'param3: {args.param3}')

	if args.param3 != '':
		compile_path = args.param3
		print(f'Compile path is changed to: {compile_path}')

	s = generate_code(args.param1, args.param2)
	write_str_to_file(file, s)

	ret = subprocess.run([compile_path, file, "-o", file + ".exe"], check = True)  
	
	if ret.returncode == 0: 
		proc = subprocess.Popen("./" + file + ".exe", shell = False, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)  
		output = proc.communicate()[0]
		print(output.decode())

		proc.terminate()
		proc.wait()
		print("Subprocess exited")

	else:  
		print("Compile error: {ret.returncode")