#include "test_func.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include "tkl/kthread.h"

tkl::kthread<4096> thread1;

LOG_MODULE_REGISTER(test_func);

SHELL_SUBCMD_SET_CREATE(sub_sectoin_cmd, (section_cmd)) = {};

static void thread1Fn(void* p1, void* p2, void* p3)
{
    while(1)
    {
        LOG_INF("1 Minutes Pass");
        k_sleep(K_SECONDS(60));
    }
}

static int cmd1_handler(const struct shell* sh, size_t argc, char** argv)
{
    ARG_UNUSED(sh);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    bool ret = true;

    ret = thread1.Create(thread1Fn);
    if(ret)
    {
        thread1.Setname("thread1");
        printf("Thread Runnting now\r\n");
    }
    else
    {
        printf("Failed to Create Thread => Already Created\r\n");
    }
    return 0;
}

SHELL_SUBCMD_SET_CREATE(sub_section_cmd1, (section_cmd, cmd1)) = {};
SHELL_SUBCMD_ADD((section_cmd), cmd1, &sub_section_cmd1, "help for cmd1", cmd1_handler, 1, 0);

static int cmd2_handler(const struct shell* sh, size_t argc, char** argv)
{
    ARG_UNUSED(sh);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    printf("Thread Aborting now\r\n");
    thread1.Abort();

    return 0;
}

SHELL_SUBCMD_SET_CREATE(sub_section_cmd2, (section_cmd, cmd2)) = {};
SHELL_SUBCMD_ADD((section_cmd), cmd2, &sub_section_cmd2, "help for cmd2", cmd2_handler, 1, 0);

static int cmd3_handler(const struct shell* sh, size_t argc, char** argv)
{
    ARG_UNUSED(sh);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    printf("Thread Starting now\r\n");
    thread1.Start();

    return 0;
}

SHELL_SUBCMD_SET_CREATE(sub_section_cmd3, (section_cmd, cmd3)) = {};
SHELL_SUBCMD_ADD((section_cmd), cmd3, &sub_section_cmd3, "help for cmd3", cmd3_handler, 1, 0);

SHELL_CMD_REGISTER(section_cmd, &sub_sectoin_cmd, "Demo for subcommand", NULL);