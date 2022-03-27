#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <regex.h>

#define PANIC_AND_EXIT(code, format, ...) do {\
        fprintf(stderr, format, ## __VA_ARGS__);\
        exit(code);\
}while(0)

#define HELP_AND_EXIT(code) PANIC_AND_EXIT(code, "%s usage: [-v|--verbose] [-h|--help] <tm> <input>", argv[0])

int verbose = 0;
static char* input_file_path;
static char* input;

typedef enum {
        TK_NOTYPE = 256,
        Q, S, G, Q0, B, F, SYMBOL, EQ, LBRACE, RBRACE, COMMA } token_type;

struct token {
        token_type type;
        char str[64];
        int line;
};

static int parse_args(int argc, char* argv[]) {
        static struct option long_options[] = {
          {"verbose", no_argument, &verbose, 'v'},
          {"help"   , no_argument, NULL    , 'h'},
          {0        , 0          , 0       ,  0 },
        };
        int o;
        while ( ( o = getopt_long(argc, argv, "vh", long_options, NULL) ) != -1 ) {
                switch(o) {
                case 'v':
                        break;
                case 'h':
                        HELP_AND_EXIT(0);
                        break;
                default:
                        exit(-1);
                }
        }
        
        /* Deal with non-option arguments here */
        if (optind < argc - 1) {
                input_file_path = argv[optind];
                input = argv[optind+1];
                struct stat statbuf;
                if(stat(input_file_path, &statbuf)){
                        PANIC_AND_EXIT(-1, "%s: %s: %s\n", argv[0], input_file_path, strerror(errno));
                }
        }else {
                HELP_AND_EXIT(-1);
        }
        return 0;
}

/*
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},    // spaces
};
*/

static int scanner(){
        int position;
        char line_buf[512];
        FILE *fin = fopen(input_file_path, "r");
        while(fgets(line_buf, sizeof(line_buf), fin)){
                int len = strlen(line_buf);
                if(len == 0){
                        continue;
                }
                position += len;
        }
        fclose(fin);
        return 0;
}

static int run(){
        scanner();
        return 0;
}

int main(int argc, char *argv[]) {
        parse_args(argc, argv);
        run();
        return 0;
}
