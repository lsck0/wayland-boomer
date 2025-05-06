#pragma once

#ifndef __ARGS_H__
#define __ARGS_H__

typedef struct {
  char* program_name;
} Args;

Args parse_commandline_arguments(int argc, char** argv);

#endif // __ARGS_H__
