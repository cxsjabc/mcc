#include <assert.h>
#include <errno.h>
#include <stdio.h>
#ifdef _WINDOWS
#include <process.h>
#else
#include <sys/wait.h>
#endif
#include <unistd.h> 

#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/exec.h"

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

#if defined(_WINDOWS) // Windows
	arr = create_dynamic_array(4);
	assert(arr);
	dynamic_array_push(arr, (void *)prog);
#if defined(__GNUC__)
	dynamic_array_push(arr, "-E");
#else
	dynamic_array_push(arr, "/E"); // cl uses "/E" instead of "-E"
#endif
	dynamic_array_push(arr, (void *)file);
	dynamic_array_push(arr, NULL);

	cmds = (char **)arr->data;
	argv = cmds;
	r = _spawnvp(_P_WAIT, CL_NAME, (const char *const *)argv);
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
		debug("child starting...\n");

		arr = create_dynamic_array(4);
		assert(arr);
		dynamic_array_push(arr, exec_path);
		dynamic_array_push(arr, "-E");
		dynamic_array_push(arr, (void *)file);
		dynamic_array_push(arr, "> __temp.i");
		dynamic_array_push(arr, NULL);

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