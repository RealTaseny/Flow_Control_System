//
// Created by Taseny on 25-4-11.
//

#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C" {
#include <stdint.h>
#endif

#define DISPLAY_LINE_HEIGHT 16U
#define MAX_DISPLAY_ITEM 5U
#define MAX_DISPLAY_CHAR 27U

void* menu_builder();
void menu_delete(void *obj);
void menu_key_pressed(void *obj, uint8_t key_value);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include <cstring>
#include <string>
#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <cmsis_os2.h>

class menuItem {
	public:
	const char* name;
	menuItem* parent;
	menuItem** children;
	uint8_t childrenCount;
	void (*appEntry)(void*);      // App入口函数
	void* appArgs;                // App参数
	TaskHandle_t appTaskHandle;   // App任务句柄
	QueueHandle_t appExitQueue = nullptr;   // 退出信号队列
	menuItem(const char* name, menuItem** children, uint8_t count, void (*entry)(void*), void* args = nullptr);

	void getappExitQueue() {
		if (appExitQueue) return;
		appExitQueue = xQueueCreate(1, sizeof(bool));
	}
};

class menuNavigator {
	private:
	menuItem* currentMenu;
	bool inAppMode;             // 是否处于叶节点模式
	menuItem* savedParent;       // 保存进入叶节点前的父菜单
	uint8_t savedSelectedIndex;  // 保存进入叶节点前的选中索引
	uint8_t savedFirstVisibleItem; // 保存进入叶节点前的可视项起始位
	uint8_t selectedIndex = 0;
	uint8_t firstVisibleItem = 0;
	char displayBuffer[MAX_DISPLAY_CHAR * MAX_DISPLAY_ITEM] = {0};

	public:
	explicit menuNavigator();
	void refreshDisplay();
	void updateDisplay();
	void handleInput(uint8_t key);
	void getDisplayInfo(char* buffer, uint8_t lineCount);
};

#endif

#endif //MENU_H
