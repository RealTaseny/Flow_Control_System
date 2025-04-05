/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void fatfs_init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  retUSER = f_mount(&USERFatFS,  "FLSH:",  1);   //挂载文件系统
  if (retUSER == FR_OK)
      ips160_show_string(0, 0, "Mounted success", RGB565_WHITE, RGB565_RED);
  else if (retUSER == FR_NO_FILESYSTEM)
  {
#if USE_IPS_ASSERT == 1
      ips160_show_string(0, 0, "FR_NO_Filesystem Error\nmake filesystem...", RGB565_WHITE, RGB565_RED);
#endif
      BYTE *work = malloc(sizeof(BYTE) * _MAX_SS);
      retUSER = f_mkfs("FLASH:", FM_FAT, _MAX_SS, work, _MAX_SS);
      free(work);
#if USE_IPS_ASSERT == 1
      if (retUSER == FR_OK) ips160_show_string(0, 24, "Make filesystem done\ntrying to remount SPI Flash...", RGB565_WHITE, RGB565_RED);
      else
      {
          ips160_show_string(0 ,24, "Make filesystem error\nerror code:", RGB565_WHITE, RGB565_RED);
          ips160_show_uint(66 , 36, retUSER, 2);
      }
#endif
    retUSER = f_mount(&USERFatFS, "FLASH:", 1);
#if USE_IPS_ASSERT == 1
    if (retUSER == FR_OK) ips160_show_string(0 , 48, "Successfully mount filesystem", RGB565_WHITE, RGB565_RED);
    else
    {
        ips160_show_string(0 , 48,"Mount filesystem error\nerror code:", RGB565_WHITE, RGB565_RED);
        ips160_show_uint(66 , 60, retUSER, 2);
    }
#endif
  }
#if USE_IPS_ASSERT == 1
  else
  {
      ips160_show_string(0 , 0,"Mount filesystem error\nerror code:", RGB565_WHITE, RGB565_RED);
      ips160_show_uint(66 , 12, retUSER, 2);
  }
#endif
  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
