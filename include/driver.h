#ifndef DRIVER_H
#define DRIVER_H

#include <common.h>

typedef int (*driver_init_t)();
typedef int (*driver_fini_t)();

struct driver_interface_table {
  driver_init_t initialize;
  driver_fini_t finalize;
};

struct driver_interface {
  const char * name;
  struct driver_interface_table functions;
};

void register_driver(struct driver_interface * drv);
void unregister_driver(struct driver_interface * drv);

#endif
