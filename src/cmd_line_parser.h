#ifndef CMD_LINE_PARSER
#define CMD_LINE_PARSER

extern char *server_cmd[];
extern char *client_cmd[];

void get_cmds(int argc, char **argv);

#endif // CMD_LINE_PARSER