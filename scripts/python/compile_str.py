# Usage: python scripts\python\compile_str.py sizeof(int) int

import argparse
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
	
	args = parser.parse_args()  
	
	print(f'param1: {args.param1}')  
	print(f'param2: {args.param2}')

	s = generate_code(args.param1, args.param2)
	write_str_to_file(file, s)

	ret = subprocess.run([compile_path, file, "-o", file + ".exe"], check = True)  
	
	if ret.returncode == 0: 
		completed_process = subprocess.Popen(file + ".exe", shell = False, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)  
		output = completed_process.communicate()[0]
		print(output.decode())
	else:  
		print("Compile error: {ret.returncode")