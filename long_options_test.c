#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int opt;
  char *name = NULL;

  // 定义长选项
  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"name", required_argument, 0, 'n'},
                                         {0, 0, 0, 0}};

  // 解析命令行参数
  while ((opt = getopt_long(argc, argv, "hn:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'h':
      printf("Usage: %s [-h] [--help] [-n name] [--name name]\n", argv[0]);
      exit(EXIT_SUCCESS);
    case 'n':
      name = optarg;
      break;
    case '?':
      // getopt_long 已经打印了错误信息
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  if (name != NULL) {
    printf("Hello, %s!\n", name);
  } else {
    printf("Hello, world!\n");
  }

  return EXIT_SUCCESS;
}