#include <assert.h>
#include <stdio.h> 
#include <sys/wait.h> 
#include <unistd.h> 

#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/exec.h"

#define FORCE_DEBUG

__BEGIN_DECLS

#ifdef FORCE_DEBUG
const static char *exec_args[] = { "notepad", NULL };
#endif

int preprocess(const char *file)
{
	pid_t p;
	int r = ERR_NONE, status;
	DynArray arr;
	char **cmds;

	p = fork();
	if (p < 0) {
		error("Fork failed.\n");
		r = ERR_FORK;
	} else if (p == 0) { // child process
		char *exec_path = GCC_NAME;
		char **argv;
		debug("child starting...\n");
		arr = create_dynamic_array(4);
		assert(arr);
		dynamic_array_push(arr, GCC_NAME);
		dynamic_array_push(arr, "-E");
		dynamic_array_push(arr, (void *)file);
		dynamic_array_push(arr, NULL);
		dump_dynamic_array(arr);
		cmds = (char **)arr->data;
		debug("cmds[0]: %s\n", cmds[0]);
		debug("cmds[1]: %s\n", cmds[1]);
		debug("cmds[2]: %s\n", cmds[2]);
		debug("cmds[3]: %s\n", cmds[3] != NULL ? cmds[3] : "NULL");
		argv = cmds;
#ifdef FORCE_DEBUG
		exec_path = "notepad";
		argv = (char **)exec_args;
#endif	
		r = execv(exec_path, argv);
		if (r == -1)
			error("Execv child failed: %d\n", r);
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
	return r;
}
__END_DECLS