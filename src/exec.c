#include <assert.h>
#include <errno.h>
#include <stdio.h>
#if defined(_WINDOWS) || defined(_MSC_VER)
#include <process.h>
#else
#include <sys/wait.h>
#include <unistd.h> 
#endif

#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/exec.h"
#include "mcc/file.h"

#define FORCE_DEBUG 0

__BEGIN_DECLS

#if FORCE_DEBUG
const static char *exec_args[] = { "ls", "-l", NULL };
#endif

int preprocess(const char *file, const char *prog)
{
	int r = ERR_NONE;
	DynArray arr;
	char **argv;
	char **cmds;

#if defined(_WINDOWS) || defined(_MSC_VER) // Windows (if clang or gcc on Windows are also available)
	char *pp_file = alloc_preprocessed_file_name(file);
	arr = create_dynamic_array(4);
	assert(arr);
	dynamic_array_push(arr, (void *)prog);
#if defined(_MSC_VER) && !defined(__clang__) // Windows clang-16 also defines _MSC_VER, ignore it!
	dynamic_array_push(arr, (void *)"/E"); // cl uses "/E" instead of "-E"
	dynamic_array_push(arr, (void *)"/P"); // preprocessed to file
	dynamic_array_push(arr, (void *)"/Fi:");
	dynamic_array_push(arr, (void *)pp_file);
#else
	dynamic_array_push(arr, (void *)"-E");
	dynamic_array_push(arr, (void *)"-o");
	dynamic_array_push(arr, (void *)pp_file);
#endif
	dynamic_array_push(arr, (void *)file);
	dynamic_array_push(arr, NULL);
	dump_dynamic_array(arr);

	cmds = (char **)arr->data;
	argv = cmds;
	// **
	// Because some compilers do not support latest standards, if meet _spawnvp or spawnvp not found, can change it
	// to the existing func to meet your environment!
	// **
#if defined(__clang__)
#if __clang_major__ >= 16
	r = _spawnvp(_P_WAIT, prog, (char *const *)argv);
#else // Cygwin clang-8 uses deprecated spawnvp, give some warmness to him!
	r = spawnvp(_P_WAIT, prog, (const char *const *)argv);
#endif
#else
#ifdef _SPAWNV_DEFINED
	// cl uses
	r = _spawnvp(_P_WAIT, prog, (char *const *)argv);
#else
	// gcc11 and g++11 uses
	r = spawnvp(_P_WAIT, prog, (const char *const *)argv);
#endif
#endif
	destroy_dynamic_array(arr);
#elif defined(__GNUC__) // *nix: Ubuntu or clang
	pid_t p;
	int status;

	p = fork();
	if (p < 0) {
		error("Fork failed.\n");
		r = ERR_FORK;
	} else if (p == 0) { // child process
		char *exec_path = (char *)prog;
		char *pp_file = alloc_preprocessed_file_name(file);
		debug("child starting...\n");

		arr = create_dynamic_array(4);
		assert(arr);
		dynamic_array_push(arr, (void *)exec_path);
		dynamic_array_push(arr, (void *)"-E");
		dynamic_array_push(arr, (void *)file);
		dynamic_array_push(arr, (void *)"-o");
		dynamic_array_push(arr, (void *)pp_file);
		dynamic_array_push(arr, NULL);
		dump_dynamic_array(arr);

		cmds = (char **)arr->data;
		argv = cmds;
#if FORCE_DEBUG
		exec_path = "ls";
		argv = (char **)exec_args;
		debug("exec_path: %s, argv[0]: %s, argv[1]: %s\n", exec_path, argv[0], argv[1]);
#endif
		r = execvp(exec_path, argv);
		if (r == -1) {
			error("Execv child failed: %d, errno: %d\n", r, errno);
			destroy_dynamic_array(arr);
		}
		else
			error("Child never execute here\n");
	} else { // parent process
		printf("Parent running...\n");
		r = waitpid(p, &status, 0);
		if (r == -1)
			error("Wait child exited failed: %d\n", r);
		else
			info("Wait for child exited with %d\n", status);
	}
#else
#error "No supported architecture"
#endif
	return r;
}
__END_DECLS