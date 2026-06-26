#include "Can.h"
#include "Can_Cfg.h"
#include "Can_Priv.h"

static Can_ChannelContextType Can_ChannelContexts[1];

static Can_ChannelConfigType Can_ChannelConfigs[1] = {
  {
    &Can_ChannelContexts[0],
    NULL,           /* CtrlPins */
    500000,         /* baudrate - 500 kbps */
    75,             /* samplePoint % */
    0,              /* hwInstanceId - CAN0 */
    0,              /* numOfCtrlPins */
    0,              /* TrcvPinSTB */
    STD_LOW,        /* NormalValueOfTrcvPinSTB */
  },
};

static uint8_t Can_hwIns2ChlMap[1] = {
  0,
};

Can_ConfigType Can_Config = {
  Can_ChannelConfigs,
  Can_hwIns2ChlMap,
  1,
  1,
};