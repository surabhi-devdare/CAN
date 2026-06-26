#include "PduR.h"
#include "PduR_Priv.h"
#include "PduR_Cfg.h"

static PduR_RoutingPathType PduR_RoutingPaths[1];

const PduR_ConfigType PduR_Config = {
  PduR_RoutingPaths,
  0
};

void PduR_CanTpReConfig(uint8_t Channel)
{
  (void)Channel;
}