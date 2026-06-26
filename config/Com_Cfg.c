#include "Com.h"
#include "Com_Cfg.h"
#include "Com_Priv.h"

static Com_GlobalContextType Com_Context;

const Com_ConfigType Com_Config = {
  NULL,
  NULL,
  &Com_Context,
  0,
  0,
  0
};