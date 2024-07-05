#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "cmd_line_parser.h"

#define SERVER_ARG_CNT (2)
#define CLIENT_ARG_CNT (2)

char * server_arg_list[SERVER_ARG_CNT];
char * client_arg_list[CLIENT_ARG_CNT];

struct node_args{
        char *server_executable;
        char *client_executable;
};

static struct node_args my_node_args = {NULL, NULL};

static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"server_executable", required_argument, 0, 's'},
        {"client_executable", required_argument, 0, 'c'},
        {0, 0, 0, 0}
};

static void print_usage(const char *prog_name) {
        printf("Desc:\t%s is a distributed peer to peer network skeleton capable of spinning both a client & a server.\n", prog_name);
        printf("\t It can run any custom client & server pair you wish for, as long as you supply the relevant executables & args.\n\n");
        printf("Usage:\t%s -s server executable -c client executable [-h]\n\n", prog_name);
        printf("\t-s server executable\tSpecify the server executable\n");
        printf("\t-c client executable\tSpecify the client executable\n");
        printf("\t-h\t\t\tDisplay this help message and exit\n");
}

static void verify_required_args(){
        //TODO: Implement.
}

static void build_args(){
        server_arg_list[0] = my_node_args.server_executable;
        server_arg_list[1] = NULL;
        client_arg_list[0] = my_node_args.client_executable;
        client_arg_list[1] = NULL;

}

static void parse_input(int argc, char **argv){
        int opt;

        while ((opt = getopt_long(argc, argv, "hs:c:", long_options, NULL)) != -1) {
                switch (opt) {
                        case 'h':
                                print_usage(argv[0]);
                                exit(EXIT_SUCCESS);
                        case 's':
                                my_node_args.server_executable = optarg;
                                break;
                        case 'c':
                                my_node_args.client_executable = optarg;
                                break;
                        default:
                                print_usage(argv[0]);
                                exit(EXIT_FAILURE);
                                }
        }
}

void get_args(int argc, char **argv){
        parse_input(argc, argv);
        verify_required_args();
        build_args();
}