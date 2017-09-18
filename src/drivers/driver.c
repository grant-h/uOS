#include <driver.h>
#include <list.h>
#include <print.h>

struct loaded_driver {
  struct driver_interface * drv;
  struct ll_node list;
};

static LIST_DEFINE(loaded_drivers);

void register_driver(struct driver_interface * drv)
{
  printf("%s: loading\n", drv->name);
  drv->functions.initialize();

  /*struct loaded_driver * driverRecord = kmalloc(sizeof(struct loaded_driver));

  driverRecord->drv = drv;
  list_append(&driverRecord->list, &loaded_drivers);*/
}

void unregister_driver(struct driver_interface * drv)
{
  printf("%s: unloading\n", drv->name);
  drv->functions.finalize();
}
