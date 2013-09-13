/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file manages the keyboard task.
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  AT90USB1287, AT90USB1286, AT90USB647, AT90USB646
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf_usb.h"
#include "keyboard_task.h"
#include "lib_board/stk_525/stk_525.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "lib_mcu/power/power_drv.h"


//_____ D E F I N I T I O N S ______________________________________________

U8  code    usb_keys[]= {HID_ENTER, \
HID_W, HID_E, HID_L, HID_C, HID_O, HID_M, HID_E, HID_SPACEBAR, HID_T, HID_O, HID_SPACEBAR, HID_H, HID_I, HID_D, HID_SPACEBAR, HID_K, HID_E, HID_Y, HID_B, HID_O, HID_A, HID_R, HID_D, HID_SPACEBAR, HID_D, HID_E, HID_M, HID_O, HID_SPACEBAR, HID_A, HID_N, HID_D, HID_SPACEBAR, HID_C, HID_O, HID_N, HID_G, HID_R, HID_A, HID_T, HID_U, HID_L, HID_A, HID_T, HID_I, HID_O, HID_N, HID_S, HID_SPACEBAR, HID_F, HID_O, HID_R, HID_SPACEBAR, HID_S, HID_E, HID_L, HID_E, HID_C, HID_T, HID_I, HID_N, HID_G, HID_SPACEBAR, HID_T, HID_H, HID_E, HID_SPACEBAR, HID_A, HID_T, HID_9, HID_0, HID_U, HID_S, HID_B, HID_X, HID_X, HID_X, HID_SPACEBAR, HID_M, HID_I, HID_C, HID_R, HID_O, HID_C, HID_O, HID_N, HID_T, HID_R, HID_O, HID_L, HID_L, HID_E, HID_R, HID_SPACEBAR, HID_F, HID_A, HID_M, HID_I, HID_L, HID_Y, HID_DOT,HID_ENTER,HID_ENTER\
,HID_T, HID_H, HID_E, HID_SPACEBAR, HID_A, HID_T, HID_9, HID_0, HID_U, HID_S, HID_B, HID_X, HID_X, HID_X, HID_SPACEBAR,HID_I, HID_S, HID_SPACEBAR, HID_B, HID_A, HID_S, HID_E, HID_D, HID_SPACEBAR,HID_O, HID_N, HID_SPACEBAR, HID_T, HID_H, HID_E, HID_SPACEBAR, HID_A, HID_V, HID_R, HID_SPACEBAR, HID_C, HID_O, HID_R, HID_E, HID_SPACEBAR, HID_W, HID_I, HID_T, HID_H, HID_SPACEBAR, HID_T, HID_H, HID_E, HID_SPACEBAR, HID_F, HID_O, HID_L, HID_L, HID_O, HID_W, HID_I, HID_N, HID_G, HID_SPACEBAR, HID_F, HID_E, HID_A, HID_T, HID_U, HID_R, HID_E, HID_S, HID_ENTER\
,HID_KEYPAD_MINUS,HID_SPACEBAR,HID_U,HID_S,HID_B,HID_SPACEBAR,HID_O,HID_T,HID_G,HID_SPACEBAR,HID_ENTER\
,HID_KEYPAD_MINUS,HID_SPACEBAR,HID_L,HID_O,HID_W,HID_SLASH,HID_F,HID_U,HID_L,HID_L,HID_SPACEBAR,HID_S,HID_P,HID_E,HID_E,HID_D,HID_SPACEBAR,HID_C,HID_O,HID_N,HID_T,HID_R,HID_O,HID_L,HID_L,HID_E,HID_R,HID_SPACEBAR,HID_W,HID_I,HID_T,HID_H,HID_SPACEBAR,HID_7,HID_SPACEBAR,HID_E,HID_N,HID_D,HID_P,HID_O,HID_I,HID_N,HID_T,HID_S,HID_ENTER\
,HID_KEYPAD_MINUS,HID_SPACEBAR,HID_U,HID_P,HID_SPACEBAR,HID_T,HID_O,HID_SPACEBAR,HID_1,HID_2,HID_8,HID_SPACEBAR,HID_K,HID_B,HID_Y,HID_T,HID_E,HID_S,HID_SPACEBAR,HID_O,HID_N,HID_KEYPAD_MINUS,HID_C,HID_H,HID_I,HID_P,HID_SPACEBAR,HID_F,HID_L,HID_A,HID_S,HID_H,HID_SPACEBAR,HID_M,HID_E,HID_M,HID_O,HID_R,HID_Y,HID_ENTER\
,HID_KEYPAD_MINUS,HID_SPACEBAR,HID_U,HID_S,HID_B,HID_SPACEBAR,HID_B,HID_O,HID_O,HID_T,HID_L,HID_O,HID_A,HID_D,HID_E,HID_R,HID_ENTER\
,HID_KEYPAD_MINUS,HID_SPACEBAR,HID_U,HID_A,HID_R,HID_T,HID_SLASH,HID_S,HID_P,HID_I,HID_SLASH,HID_SPACEBAR,HID_T,HID_W,HID_I,HID_ENTER\
,HID_SPACEBAR,HID_SPACEBAR,HID_A,HID_N,HID_D,HID_SPACEBAR,HID_M,HID_O,HID_R,HID_E,HID_DOT,HID_DOT,HID_DOT,HID_ENTER\
,HID_E,HID_N,HID_J,HID_O,HID_Y,HID_SPACEBAR,HID_Y,HID_O,HID_U,HID_R,HID_SPACEBAR,HID_T,HID_I,HID_M,HID_E,HID_SPACEBAR,HID_W,HID_I,HID_T,HID_H,HID_SPACEBAR,HID_A,HID_T,HID_M,HID_E,HID_L,HID_SPACEBAR,HID_P,HID_R,HID_O,HID_D,HID_U,HID_C,HID_T,HID_S,HID_DOT,HID_DOT,HID_DOT\
                           };

#define SIZEOF_USB_KEYS     (Uint16)sizeof(usb_keys)

U8 code usb_arrow[] = {HID_UP, HID_DOWN};
U8 code usb_vol[] = {HID_VOLUME_MUTE, HID_VOLUME_UP, HID_VOLUME_DOWN};
U8 code usb_test[] = {HID_A, HID_B, HID_C};



//_____ D E C L A R A T I O N S ____________________________________________

volatile U8    cpt_sof;
         U8    transmit_no_key;
volatile bit   key_hit;
         U8    usb_key;
         U8    usb_kbd_state;
         U16   usb_data_to_send;
#ifdef __GNUC__
PGM_VOID_P     usb_key_pointer;
#else
U8   code *    usb_key_pointer;
#endif


//! This function initializes the hardware/software ressources required for keyboard task.
//!
void keyboard_task_init(void)
{
   transmit_no_key   = FALSE;
   key_hit           = FALSE;
   usb_kbd_state     = 0;
   Joy_init();
   cpt_sof           = 0;
}


//! @brief Entry point of the mouse management
//! This function links the mouse and the USB bus.
//!
void keyboard_task(void)
{
   if(Is_device_enumerated()) {
      // if USB ready to transmit new data :
      //        - if last time = 0, nothing
      //        - if key pressed -> transmit key
      //        - if !key pressed -> transmit 0
      if (key_hit == FALSE) {
         kbd_test_hit();
      } else {
         Usb_select_endpoint(EP_KBD_IN);   
         if(Is_usb_write_enabled()) {
            if (transmit_no_key == FALSE) {
               transmit_no_key = TRUE;
               Usb_write_byte(HID_MODIFIER_NONE);  // Byte0: Modifier
               Usb_write_byte(0);                  // Byte1: Reserved
               Usb_write_byte(usb_key);            // Byte2: Keycode 0
               Usb_write_byte(0);                  // Byte2: Keycode 1
               Usb_write_byte(0);                  // Byte2: Keycode 2
               Usb_write_byte(0);                  // Byte2: Keycode 3
               Usb_write_byte(0);                  // Byte2: Keycode 4
               Usb_write_byte(0);                  // Byte2: Keycode 5
               Usb_send_in();
               return;
            } else {
               key_hit = FALSE;
               transmit_no_key = FALSE;
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_write_byte(0);
               Usb_send_in();
            }
         }
      }
   }
}

U8 keystate[] = {0, 0, 0, 0, 0};  // 0:initial    1:pressed    2:released
#define KS_UP     0
#define KS_DOWN   1
#define KS_RIGHT  2
#define KS_LEFT   3
#define KS_DUMMY  4
U8 ksnum;
bool is_pressed_down()
{
    U8 *pks = &keystate[KS_DOWN];
    U8 k = Is_joy_down();
    if((k == TRUE  ) && (*pks == 0)) *pks = 1;
    if((k == FALSE ) && (*pks == 1)) *pks = 2;

    return (*pks == 2);
}

bool is_pressed_up()
{
    U8 *pks = &keystate[KS_UP];
    U8 k = Is_joy_up();
    if((k == TRUE  ) && (*pks == 0)) *pks = 1;
    if((k == FALSE ) && (*pks == 1)) *pks = 2;

    return (*pks == 2);
}

bool is_pressed_left()
{
    U8 *pks = &keystate[KS_LEFT];
    U8 k = Is_joy_left();
    if((k == TRUE  ) && (*pks == 0)) *pks = 1;
    if((k == FALSE ) && (*pks == 1)) *pks = 2;

    return (*pks == 2);
}

bool is_pressed_right()
{
    U8 *pks = &keystate[KS_RIGHT];
    U8 k = Is_joy_right();
    if((k == TRUE  ) && (*pks == 0)) *pks = 1;
    if((k == FALSE ) && (*pks == 1)) *pks = 2;

    return (*pks == 2);
}


//! @brief Chech keyboard key hit
//! This function scans the keyboard keys and update the scan_key word.
//!   if a key is pressed, the key_hit bit is set to TRUE.
//!
void kbd_test_hit(void)
{
   switch (usb_kbd_state) {
      case 0:
      //  if(Is_btn_middle()) {
      //      usb_kbd_state = 1;
      //      usb_key_pointer = usb_keys;
      //      usb_data_to_send = SIZEOF_USB_KEYS;
      //      ksnum = KS_DUMMY;
      //  }
#if 1
          // arrow keys
          if(is_pressed_left()) {
              usb_kbd_state = 1;
              usb_key_pointer = &usb_arrow[1];
              usb_data_to_send = 1;
              ksnum = KS_LEFT;
          }
          else if(is_pressed_right()) {
              usb_kbd_state = 1;
              usb_key_pointer = &usb_arrow[0];
              usb_data_to_send = 1;
              ksnum = KS_RIGHT;
          }
#else
          // volume up/down, mute
          if(is_pressed_down()) {
              usb_kbd_state = 1;
              usb_key_pointer = &usb_vol[0];
              usb_data_to_send = 1;
              ksnum = KS_DOWN;
          }
          else if(is_pressed_right()) {
              usb_kbd_state = 1;
              usb_key_pointer = &usb_vol[1];
              usb_data_to_send = 1;
              ksnum = KS_RIGHT;
          }
          else if(is_pressed_left()) {
              usb_kbd_state = 1;
              usb_key_pointer = &usb_vol[2];
              usb_data_to_send = 1;
              ksnum = KS_LEFT;
          }
#endif
          break;

      case 1:
          if(usb_data_to_send != 0) {
              if((key_hit == FALSE) && (transmit_no_key == FALSE)) {
#ifndef __GNUC__
                  usb_key = *usb_key_pointer++;
#else
                  usb_key = pgm_read_byte_near(usb_key_pointer++);
#endif
                  usb_data_to_send--;
                  key_hit = TRUE;
              }
          } else {
              usb_kbd_state = 0;
              keystate[ksnum] = 0;
          }
          break;
   }
}


//! @brief vbus_off_action
//! This function increments the action to be executed upon
//! the USB VBUS disconnection
//! Here a Vbus lost lead to detach
//!
void vbus_off_action(void)
{
   Usb_detach();
}


void suspend_action(void)
{
#if (USB_REMOTE_WAKEUP_FEATURE == ENABLED)
   if (remote_wakeup_feature == ENABLED) {
      Switches_enable_it()
   }
#endif 
   Enable_interrupt();  
   Enter_power_down_mode();
}


#ifdef __GNUC__
 ISR(PCINT0_vect)
#else
#pragma vector = PCINT0_vect
__interrupt void mouse_disco_int()
#endif
{
   Switches_disable_it();
   usb_generate_remote_wakeup();
}

