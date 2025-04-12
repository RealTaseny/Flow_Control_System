//
// Created by Taseny on 25-4-11.
//

#include "Menu.h"
#include "ips_160.h"
#include "key.h"
#include "peripheral_init.h"
#include <stdio.h>

extern void *MENU;

void refresh_Display(void *obj);

void SystemReboot(void *arg) {  POWER_OFF(); }
void SetBacklight(void *arg) { /* 设置背光 */ }

void Test_func1(void *arg){ ips160_full(RGB565_RED); }
void Test_func2(void *arg){ printf("Test2\r\n"); }
void Test_func3(void *arg){ printf("Test3\r\n"); }
void Test_func4(void *arg){ printf("Test4\r\n"); }
void Test_func5(void *arg){ printf("Test5\r\n"); }
void Test_func6(void *arg){ printf("Test6\r\n"); }
void Test_func7(void *arg){ printf("Test7\r\n"); }

menuItem testItem1("水泵电机", nullptr, 0, Test_func1);
menuItem testItem2("文件管理", nullptr, 0, Test_func2);
menuItem testItem3("文件接收", nullptr, 0, Test_func3);
menuItem testItem4("测试1", nullptr, 0, Test_func4);
menuItem testItem5("测试2", nullptr,0, Test_func5);
menuItem testItem6("测试3", nullptr,0, Test_func6);
menuItem testItem7("测试4", nullptr,0, Test_func7);
menuItem testItem8("测试5", nullptr,0, nullptr);
menuItem testItem9("测试6", nullptr,0, nullptr);
menuItem testItem10("测试7", nullptr,0, nullptr);
menuItem testItem11("测试8", nullptr,0, nullptr);
menuItem testItem12("测试9", nullptr,0, nullptr);
menuItem testItem13("测试10", nullptr,0, nullptr);
menuItem testItem14("测试11", nullptr,0, nullptr);
menuItem testItem15("测试12", nullptr,0, nullptr);


menuItem rebootItem("关机", nullptr, 0, SystemReboot);
menuItem backlightItem("背光", nullptr, 0, SetBacklight);

menuItem* test_Children[] = {
	&testItem1,
	&testItem2,
	&testItem3,
	&testItem4,
	&testItem5,
	&testItem6,
	&testItem7,
	&testItem8,
	&testItem9,
	&testItem10,
	&testItem11,
	&testItem12,
	&testItem13,
	&testItem14,
	&testItem15
};

menuItem testMenu("测试功能", test_Children, 15, nullptr);

menuItem* settingsChildren[] = {
	&backlightItem,
	&rebootItem
};

menuItem settingsMenu("设置", settingsChildren, 2, nullptr);

menuItem* mainChildren[] = {
	&settingsMenu,
	&testMenu
};

menuItem mainMenu("Main", mainChildren, 2, nullptr);

menuItem::menuItem(const char* name, menuItem** children, uint8_t count, void (*entry)(void*), void* args)
	: name(name), parent(nullptr), children(children), childrenCount(count),
	  appEntry(entry), appArgs(args ? args : (entry ? this : nullptr)), appTaskHandle(nullptr)
{
	if (children)
	{
		for (uint8_t i = 0; i < count; ++i)
		{
			children[i]->parent = this;
		}
	}
}

menuNavigator::menuNavigator()
	: currentMenu(&mainMenu),
	  inAppMode(false),
	  savedParent(nullptr),
	  savedSelectedIndex(0),
	  savedFirstVisibleItem(0) {}

void menuNavigator::handleInput(uint8_t key) {
    if (inAppMode) {
        if (key == KEY_LEFT) {
        	bool exitSignal = true;
        	xQueueSend(currentMenu->children[savedSelectedIndex]->appExitQueue, &exitSignal, 0);
            currentMenu = savedParent;
            selectedIndex = savedSelectedIndex;
            firstVisibleItem = savedFirstVisibleItem;
        }
        return;
    }

    switch(key) {
        case KEY_UP:
            selectedIndex = (selectedIndex == 0) ?
                (currentMenu->childrenCount - 1) :
                (selectedIndex - 1);
    		if (selectedIndex == currentMenu->childrenCount - 1)
    		{
    			if (currentMenu->childrenCount > MAX_DISPLAY_ITEM) ips160_full(RGB565_BLACK);
    			firstVisibleItem = selectedIndex - (selectedIndex % MAX_DISPLAY_ITEM);
    		}
            break;

        case KEY_DOWN:
            selectedIndex = (selectedIndex + 1) % currentMenu->childrenCount;
    		if (selectedIndex == 0)
    		{
    			if (currentMenu->childrenCount > MAX_DISPLAY_ITEM) ips160_full(RGB565_BLACK);
    			firstVisibleItem = 0;
    		}
            break;

        case KEY_RIGHT: {
            menuItem* selectedItem = currentMenu->children[selectedIndex];
            if (selectedItem->childrenCount > 0)
            {

            	ips160_full(RGB565_BLACK);

            	savedParent = currentMenu;
            	savedSelectedIndex = selectedIndex;
            	savedFirstVisibleItem = firstVisibleItem;
                currentMenu = selectedItem;
                selectedIndex = 0;
            }
            else if (selectedItem->appEntry)
            {

            	ips160_full(RGB565_BLACK);

            	inAppMode = true;
            	savedParent = currentMenu;
            	savedSelectedIndex = selectedIndex;
            	savedFirstVisibleItem = firstVisibleItem;
            	if (!currentMenu->children[savedSelectedIndex]->appExitQueue) currentMenu->children[savedSelectedIndex]->getappExitQueue();

            	xTaskCreate(
					selectedItem->appEntry,							// App 入口函数
					selectedItem->name,								// 任务名称
					128 * 4,										// 堆栈大小
					selectedItem->appArgs,							// 参数
					osPriorityLow,									// 优先级
					&selectedItem->appTaskHandle					// 任务句柄
				);
            }
        }
    	break;

        case KEY_LEFT:
            if (currentMenu->parent)
            {
            	currentMenu = savedParent;
            	selectedIndex = savedSelectedIndex;
            	firstVisibleItem = savedFirstVisibleItem;
                ips160_full(RGB565_BLACK); // 返回时清屏
            }

            break;
    }

    if (currentMenu->childrenCount > MAX_DISPLAY_ITEM) {
        if (selectedIndex >= firstVisibleItem + MAX_DISPLAY_ITEM)
        {
        	ips160_full(RGB565_BLACK);
        	firstVisibleItem += MAX_DISPLAY_ITEM;
        }
    	else if (selectedIndex < firstVisibleItem && selectedIndex != 0)
    	{
    		ips160_full(RGB565_BLACK);
            firstVisibleItem -= MAX_DISPLAY_ITEM;
        }
    }
}

void menuNavigator::getDisplayInfo(char* buffer, uint8_t lineCount) {
	for(uint8_t i = 0; i < lineCount; i++)
	{
		if(i < currentMenu->childrenCount)
		{
			sprintf(&buffer[i * MAX_DISPLAY_CHAR], "%s%s\n", (i == selectedIndex - firstVisibleItem) ? "->" : "  ", currentMenu->children[i + firstVisibleItem]->name);
		}
	}
}

void menuNavigator::refreshDisplay() {
	inAppMode = false;
	if (currentMenu->childrenCount - firstVisibleItem < MAX_DISPLAY_ITEM)
	{
		this->getDisplayInfo(displayBuffer, (currentMenu->childrenCount % MAX_DISPLAY_ITEM));
		for (uint8_t i = 0; i < (currentMenu->childrenCount % MAX_DISPLAY_ITEM); i++) {
			ips160_show_string(0, i * DISPLAY_LINE_HEIGHT,
							  &displayBuffer[i * MAX_DISPLAY_CHAR],
							  RGB565_BLACK, RGB565_WHITE, 1);
		}
	}
	else
	{
		this->getDisplayInfo(displayBuffer, (currentMenu->childrenCount > MAX_DISPLAY_ITEM ? MAX_DISPLAY_ITEM : currentMenu->childrenCount));
		for (uint8_t i = 0; i < (currentMenu->childrenCount > MAX_DISPLAY_ITEM ? MAX_DISPLAY_ITEM : currentMenu->childrenCount); i++) {
			ips160_show_string(0, i * DISPLAY_LINE_HEIGHT,
							  &displayBuffer[i * MAX_DISPLAY_CHAR],
							  RGB565_BLACK, RGB565_WHITE, 1);
		}
	}
}

void menuNavigator::updateDisplay() {
	if (inAppMode) {
		return;
	}

	if (currentMenu->childrenCount - firstVisibleItem < MAX_DISPLAY_ITEM)
	{
		this->getDisplayInfo(displayBuffer, (currentMenu->childrenCount % MAX_DISPLAY_ITEM));
		for (uint8_t i = 0; i < (currentMenu->childrenCount % MAX_DISPLAY_ITEM); i++) {
			ips160_show_string(0, i * DISPLAY_LINE_HEIGHT,
							  &displayBuffer[i * MAX_DISPLAY_CHAR],
							  RGB565_BLACK, RGB565_WHITE, 1);
		}
	}
	else
	{
		this->getDisplayInfo(displayBuffer, (currentMenu->childrenCount > MAX_DISPLAY_ITEM ? MAX_DISPLAY_ITEM : currentMenu->childrenCount));
		for (uint8_t i = 0; i < (currentMenu->childrenCount > MAX_DISPLAY_ITEM ? MAX_DISPLAY_ITEM : currentMenu->childrenCount); i++) {
			ips160_show_string(0, i * DISPLAY_LINE_HEIGHT,
							  &displayBuffer[i * MAX_DISPLAY_CHAR],
							  RGB565_BLACK, RGB565_WHITE, 1);
		}
	}
}

void refresh_Display(void *obj)
{
	ips160_full(RGB565_BLACK);
	const auto menu = static_cast<menuNavigator*>(obj);
	menu->refreshDisplay();
	delete menu;
}

void* menu_builder()
{
	const auto menu = new menuNavigator();
	menu->updateDisplay();
	return menu;
}

void menu_delete(void *obj)
{
	delete static_cast<menuNavigator*>(obj);
}

void menu_key_pressed(void *obj, const uint8_t key_value)
{
	const auto menu = static_cast<menuNavigator*>(obj);
	menu->handleInput(key_value);
	menu->updateDisplay();
}

