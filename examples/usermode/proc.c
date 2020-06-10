#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <amigahunk.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "proc.h"

Proc_t *TaskGetProc(void) {
  return pvTaskGetThreadLocalStoragePointer(NULL, TLS_PROC);
}

void TaskSetProc(Proc_t *proc) {
  vTaskSetThreadLocalStoragePointer(NULL, TLS_PROC, (void *)proc);
}

int ProcLoadImage(Proc_t *proc, File_t *exe) {
  Hunk_t *hunk = LoadHunkList(exe);
  FileClose(exe);

  if (hunk == NULL)
    return 0;
  proc->hunk = hunk;
  return 1;
}

void ProcFreeImage(Proc_t *proc) {
  Hunk_t *next;
  for (Hunk_t *hunk = proc->hunk; hunk != NULL; hunk = next) {
    next = hunk->next;
    free(hunk);
  }
}

void ProcInit(Proc_t *proc, size_t ustksz) {
  static int pid = 1; /* let's assume it will never overflow */

  bzero(proc, sizeof(Proc_t));

  /* Align to long word size. */
  ustksz = (ustksz + 3) & -4;
  proc->ustksz = ustksz;
  proc->ustk = malloc(ustksz);
  bzero(proc->ustk, ustksz);

  proc->pid = pid++;
}

void ProcFini(Proc_t *proc) {
  ProcFreeImage(proc);

  for (int i = 0; i < MAXFILES; i++) {
    File_t *f = proc->fdtab[i];
    if (f != NULL)
      FileClose(f);
  }

  free(proc->ustk);
}

#define PUSH(sp, v)                                                            \
  {                                                                            \
    uint32_t *__sp = sp;                                                       \
    *(--__sp) = (uint32_t)v;                                                   \
    sp = __sp;                                                                 \
  }

static void *CopyArgVec(void *sp, char *const *argv) {
  /* Copy argv contents to user stack and create argc and argv. */
  char **uargv;
  char *uargs;
  int argc;

  for (argc = 0, uargs = sp; argv[argc]; argc++)
    uargs -= strlen(argv[argc]) + 1;

  /* align stack pointer to long word boundary */
  uargs = (void *)((intptr_t)uargs & ~3);
  /* make space for NULL terminated pointer array */
  uargv = (char **)uargs - (argc + 1);

  for (int i = 0; i < argc; i++) {
    int sz = strlen(argv[i]) + 1;
    uargv[i] = memcpy(uargs, argv[i], sz);
    uargs += sz;
  }

  uargv[argc] = NULL; /* argv terminator */

  /* push pointer to uargv and argc on stack */
  sp = uargv;
  PUSH(sp, uargv);
  PUSH(sp, argc);
  return sp;
}

void ProcExecute(Proc_t *proc, char *const *argv) {
  /* Execute assumes that _start procedure is placed
   * at the beginning of first hunk of executable file. */
  void *pc = proc->hunk->data;
  /* Stack grows down. */
  void *sp = CopyArgVec(proc->ustk + proc->ustksz, argv);

  if (!setjmp(proc->retctx))
    EnterUserMode(pc, sp);
}

__noreturn void ExitUserMode(Proc_t *proc, int exitcode) {
  proc->exitcode = exitcode;
  longjmp(proc->retctx, 1);
}

int ProcFileInstall(Proc_t *proc, int fd, File_t *file) {
  if (fd < 0 || fd >= MAXFILES)
    return 0;

  File_t **fp = &proc->fdtab[fd];
  if (*fp)
    FileClose(*fp);
  *fp = file;
  return 1;
}
