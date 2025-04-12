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
#include "w25qxx_flash.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */
FIL fontFile;

/* USER CODE BEGIN Variables */
#if ENABLE_YMODEM == 1

/**
 * @brief 自动创建父目录并新建文件
 * @param path 文件路径（如 "1:/testdir1/subdir/test.txt"）
 * @return FRESULT FatFs错误码（FR_OK表示成功）
 */
FRESULT create_file(const char* path) {
    FRESULT fr;
    FATFS fs;
    FIL file;
    char dir_path[256]; // 根据实际需求调整缓冲区大小
    char *p;

    // 复制路径到缓冲区，避免修改原字符串
    strncpy(dir_path, path, sizeof(dir_path));
    dir_path[sizeof(dir_path) - 1] = '\0';

    // 去除文件名部分，只保留目录路径
    p = strrchr(dir_path, '/');
    if (p != NULL) {
        *p = '\0'; // 截断目录部分
    } else {
        // 路径中没有目录，直接创建文件
        return f_open(&file, path, FA_CREATE_NEW | FA_WRITE);
    }

    // 处理根目录（如 "1:/"）
    if (strlen(dir_path) <= 3) { // 根目录长度（如 "1:/"）
        return f_open(&file, path, FA_CREATE_NEW | FA_WRITE);
    }

    // 逐级创建目录
    p = dir_path;
    if (*(p + 1) == ':') { // 跳过驱动器号（如 "1:/"）
        p += 3;
    }

    while (*p != '\0') {
        char *slash = strchr(p, '/');
        if (slash != NULL) {
            *slash = '\0'; // 临时截断路径
        }

        // 检查当前目录是否存在
        fr = f_stat(dir_path, NULL);
        if (fr == FR_NO_FILE) { // 目录不存在，创建它
            fr = f_mkdir(dir_path);
            if (fr != FR_OK && fr != FR_EXIST) {
                return fr; // 创建失败
            }
        }

        if (slash != NULL) {
            *slash = '/'; // 恢复路径分隔符
            p = slash + 1;
        } else {
            break;
        }
    }

    // 创建文件
    fr = f_open(&file, path, FA_CREATE_NEW | FA_WRITE);
    if (fr == FR_OK) {
        f_close(&file);
    }

    return fr;
}

/**
  * @brief  递归删除目录及其内容（类似 rm -r）
  * @param  path: 要删除的目录路径（必须符合FATFS格式，例如 "0:/folder"）
  * @retval FRESULT: FATFS操作结果
  */
FRESULT delete_files(const char* path) {
    FRESULT res;
    DIR dir;
    FILINFO fno;
    char* child_path = NULL;

    // 尝试直接删除（如果是文件或空目录）
    res = f_unlink(path);
    if (res == FR_OK) return FR_OK; // 删除成功
    if (res != FR_DENIED && res != FR_INVALID_NAME) return res; // 非目录错误直接返回

    // 需要递归删除内容：打开目录
    res = f_opendir(&dir, path);
    if (res != FR_OK) return res;

    // 遍历目录项
    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break; // 错误或遍历完成

        // 跳过 "." 和 ".."
        if (strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0) {
            continue;
        }

        // 动态分配子路径内存（路径长度 + 文件名 + 2字节分隔符/终止符）
        size_t path_len = strlen(path);
        child_path = malloc(path_len + strlen(fno.fname) + 2);
        if (!child_path) {
            res = FR_NOT_ENOUGH_CORE;
            break;
        }

        // 拼接子路径（自动处理斜杠）
        if (path[path_len - 1] == '/') {
            sprintf(child_path, "%s%s", path, fno.fname);
        } else {
            sprintf(child_path, "%s/%s", path, fno.fname);
        }

        // 递归处理子项
        res = delete_files(child_path);
        free(child_path);
        child_path = NULL;
        if (res != FR_OK) break;
    }

    // 关闭目录（无论遍历是否成功）
    f_closedir(&dir);

    // 最后删除当前目录（如果前面步骤成功）
    if (res == FR_OK) {
        res = f_unlink(path);
    }

    return res;
}

/**
 * @brief 递归扫描目录并打印树形结构
 * @param path 当前目录路径（必须以'/'结尾）
 * @param depth 当前递归深度（用于缩进，初始为0）
 * @return FRESULT FatFs错误码
 */
FRESULT scan_tree(const char* path, uint8_t depth) {
    FRESULT res;
    DIR dir;
    FILINFO fno;
    char *temp_path = malloc(strlen(path) + 2); // 路径缓冲区（根据实际情况调整大小）
    uint8_t i;

    // 打开当前目录
    res = f_opendir(&dir, path);
    if (res != FR_OK) return res;

    // 读取目录项
    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;

        // 忽略 "." 和 ".."
        if (strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0) {
            continue;
        }

        // 打印缩进
        for (i = 0; i < depth; i++) {
            printf("    "); // 每层缩进4空格
        }

        // 拼接完整路径
        snprintf(temp_path, sizeof(temp_path), "%s%s", path, fno.fname);
        free(temp_path);
        temp_path = NULL;
        if (fno.fattrib & AM_DIR) {
            // 打印目录项（黄色）
            printf("\033[33m%s/\033[0m\n", fno.fname);

            // 递归处理子目录（路径后需加'/'）
            char *child_path = malloc(strlen(path) + strlen(fno.fname) + 1);

            sprintf(child_path, "%s/%s", path, fno.fname);
            res = scan_tree(child_path, depth + 1);
            free(child_path);
            child_path = NULL;

            if (res != FR_OK) break;
        } else {
            // 打印文件项（绿色）
            printf("\033[32m%s\033[0m\n", fno.fname);
        }
    }

    f_closedir(&dir);
    return res;
}

// 用户调用入口
FRESULT tree(const char* root_path) {
    return scan_tree(root_path, 0);
}

#endif
/* USER CODE END Variables */

void fatfs_init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  retUSER = f_mount(&USERFatFS,  "FLASH:",  1);   //挂载文件系统
  if (retUSER == FR_OK)
  {
#if USE_IPS_ASSERT == 1
      ips160_show_string(0, 0, "Mounted success", RGB565_WHITE, RGB565_RED, 0);
      HAL_Delay(DISPLAY_DELAY);
#endif
      (void)retUSER;
  }
  else if (retUSER == FR_NO_FILESYSTEM)
  {
#if USE_IPS_ASSERT == 1
      ips160_show_string(0, 0, "FR_NO_Filesystem Error make filesystem...", RGB565_WHITE, RGB565_RED, 0);
#endif
      BYTE *work = malloc(sizeof(BYTE) * _MAX_SS);
      retUSER = f_mkfs("FLASH:", FM_FAT, _MAX_SS, work, _MAX_SS);
      free(work);
      work = NULL;
#if USE_IPS_ASSERT == 1
      if (retUSER == FR_OK) ips160_show_string(0, 24, "Make filesystem done trying to remount SPI Flash...", RGB565_WHITE, RGB565_RED, 0);
      else
      {
          sprintf(display_string_buffer, "Make filesystem error code:%d", retUSER);
          ips160_show_string(0 ,24, display_string_buffer, RGB565_WHITE, RGB565_RED, 0);
      }
#endif
    retUSER = f_mount(&USERFatFS, "FLASH:", 1);
#if USE_IPS_ASSERT == 1
    if (retUSER == FR_OK) ips160_show_string(0 , 48, "Successfully mount filesystem", RGB565_WHITE, RGB565_RED, 0);
    else
    {
        sprintf(display_string_buffer, "Mount filesystem error code:%d", retUSER);
        ips160_show_string(0 , 48,display_string_buffer, RGB565_WHITE, RGB565_RED, 0);
    }
      HAL_Delay(DISPLAY_DELAY);
#endif
  }
#if USE_IPS_ASSERT == 1
  else
  {
      ips160_show_string(0 , 0,"Mount filesystem error\nerror code:", RGB565_WHITE, RGB565_RED, 0);
      ips160_show_uint(66 , 12, retUSER, 2);
      HAL_Delay(DISPLAY_DELAY);
  }
#endif

  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
