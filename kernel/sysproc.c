#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
extern int countFreeSpace();
extern int activeProcessStat();

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// hello world!
uint64
sys_hello(void)
{
  char buf[512];
  argstr(0,buf,512);
  printf("hello from kernel space! nice to know you %s!\n",buf);
  return 0;
}

// trace
uint64
sys_trace(void)
{
  int mask = 0;
  argint(0,&mask);
  myproc()->tracemask = mask;
  // printf("trace mask %d set\n",mask);
  return 0;
}

uint64
sys_sysinfo(void)
{
  // read in pointer argument
  uint64 addr = 0;
  argaddr(0,&addr);
  // read sysinfo
  struct proc *p = myproc();
  struct sysinfo info;
  info.freemem = countFreeSpace();
  info.nproc = activeProcessStat();
  // pass result back to user space
  if(copyout(p->pagetable,addr,(char *)&info,sizeof(info)) < 0) {
    return -1;
  }
  // printf("sysinfo\n");
  return 0;
}