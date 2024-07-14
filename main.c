// STANDARD LIBRARY
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// EXTERNAL LIBRARIES
#include <dotenv.h>
#include <rte_common.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_ethdev.h>

// PROJECT LIBRARY
#include "general.h"
#include "macros.h"

void handle_sigint(int sig) { LOGI("SIG", "caught signal %d\n"); }

int main(int argc, char **argv) {
    LOGI("AUTHOR", "%s\n", nt_project_author());

    if (dotenv_load(".env") != 0) {
        LOGE("ENV", "couldn't load .env file\n");
        return EXIT_FAILURE;
    }

    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_panic("error: coulnd't initialize EAL");
    }
    argc -= ret;
    argv += ret;

    uint16_t ports_count = rte_eth_dev_count_avail();
    if (ports_count == 0) {
        LOGE("DEVICES", "ethernet ports are not available");
        rte_eal_cleanup();
        rte_exit(EXIT_FAILURE, "exiting...");
    }
    if (IS_ODD(ports_count)) {
        LOGE("DEVICES", "count of ethernet ports is even number => ports must "
                        "be in pairs [Tx, Rx]");
        rte_eal_cleanup();
        rte_exit(EXIT_FAILURE, "exiting...");
    }

    signal(SIGINT, handle_sigint);

    if (rte_eal_cleanup() != 0) {
        LOGE("EAL", "couldn't release taken resourses");
        return EXIT_FAILURE;
    }
    LOGD("EAL", "released resources successfully");

    LOGD("Application", "exiting successfully");
    return EXIT_SUCCESS;
}
