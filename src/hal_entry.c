/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
#include "common_utils.h"

/*******************************************************************************************************************//**
 * @addtogroup CAN_EP
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define WAIT_TIME                       (5000U)            //wait time value
#define CAN_DESTINATION_MAILBOX_3       (3U)               //destination mail box number
#define CAN_MAILBOX_NUMBER_0            (0U)               //mail box number
#define CAN_FRAME_TRANSMIT_DATA_BYTES   (8U)               //data length
#define ZERO                            (0U)
#define NULL_CHAR                       ('\0')
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/* Flags, set from Callback function */
static volatile bool b_can_tx = false;                  //CAN transmission status
static volatile bool b_can_rx = false;                  //CAN receive status
static volatile bool b_can_err = false;                 //CAN error status
/* CAN frames for tx and rx */
static can_frame_t g_can_tx_frame = {RESET_VALUE};      //CAN transmit frame
static can_frame_t g_can_rx_frame = {RESET_VALUE};      //CAN receive frame

/***********************************************************************************************************************
 * Private local functions
 **********************************************************************************************************************/
static void can_deinit(void);
void R_BSP_WarmStart(bsp_warm_start_event_t event);

/*******************************************************************************************************************//**
 * The RA Configuration tool generates main() and uses it to generate threads if an RTOS is used.  This function is
 * called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t time_out = WAIT_TIME;                                      // time out
    fsp_pack_version_t version = {RESET_VALUE};
    uint8_t can_tx_msg[CAN_FRAME_TRANSMIT_DATA_BYTES] = "TX__MESG";       //data to be load in tx_frame while transmitting

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch );
    APP_PRINT("\r\nCAN example: auto-transmit same frame on bus every 100 ms.\r\n");

#if defined (BOARD_RA2A1_EK) || defined (BOARD_RA2L1_EK)
    err = R_CGC_Open (&g_cgc0_ctrl, &g_cgc0_cfg);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\nCGC Open API failed");
        APP_ERR_TRAP(err);
    }
    err = R_CGC_ClockStart (&g_cgc0_ctrl, CGC_CLOCK_MAIN_OSC, NULL);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\nCGC ClockStart API failed");
        APP_ERR_TRAP(err);
    }
#endif
    /* Initialize CAN module */
    err = R_CAN_Open(&g_can_ctrl, &g_can_cfg);
    /* Error trap */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\nCAN Open API failed");
        APP_ERR_TRAP(err);
    }

    g_can_tx_frame.id = CAN_DESTINATION_MAILBOX_3;
    g_can_tx_frame.type = CAN_FRAME_TYPE_DATA;
    g_can_tx_frame.data_length_code = CAN_FRAME_TRANSMIT_DATA_BYTES;

    /* copy the tx data frame with TX_MSG */
    memcpy((uint8_t*)&g_can_tx_frame.data[ZERO], (uint8_t*)&can_tx_msg[ZERO], CAN_FRAME_TRANSMIT_DATA_BYTES);

    APP_PRINT("\r\nCAN auto-transmit started (100 ms period)\r\n");

    while (true)
    {
        err = R_CAN_Write(&g_can_ctrl, CAN_MAILBOX_NUMBER_0, &g_can_tx_frame);
        if (FSP_SUCCESS != err)
        {
            APP_ERR_PRINT("\r\n CAN Write API FAILED");
            can_deinit();
            APP_ERR_TRAP(err);
        }

        time_out = WAIT_TIME;
        while ((true != b_can_tx) && (--time_out));
        if (RESET_VALUE == time_out)
        {
            APP_ERR_PRINT("CAN transmission failed due to timeout");
            APP_ERR_TRAP(true);
        }
        b_can_tx = false;

        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/*******************************************************************************************************************//**
 * This function is called when an CAN event is occurred and SET the respective flags.
 **********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @brief       This function is to de-initializes the CAN module
 * @param[in]   None
 * @return      None
 **********************************************************************************************************************/
static void can_deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;
    err = R_CAN_Close(&g_can_ctrl);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n **CAN Close API failed**");
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event) {
    if (BSP_WARM_START_POST_C == event) {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}

/*******************************************************************************************************************//**
 * @} (end defgroup CAN_EP)
 **********************************************************************************************************************/
