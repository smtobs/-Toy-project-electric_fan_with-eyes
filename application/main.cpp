#include <iostream>
#include <thread>
#include "unistd.h"
#include "lcd.hpp"
#include "keyPad.hpp"
#include "systemManager.hpp"

#define PASSWORD  '@'
#define CALL      '&'
#define NULL_DATA 0

bool is_video_event;

void SendVideo()
{
    uint8_t time_out = 0;
    for (;;)
    {
        if (is_video_event)
        {
            if (++time_out < 60)
            {
                std::cout << "비디오 이미지 전송중! \n";
            }
            else
            {
                is_video_event = false;
                time_out = 0;
            }
        }
        sleep(1);
	}
}

int main()
{
    std::vector<char> test1 = {'1', '2', '3', '4'};
    KeyPad *key_pad = new KeyPad();
    Lcd *lcd = new Lcd();
    SystemManager *sys_mgr = new SystemManager();
    
    if (sys_mgr->PwCompare(test1))
    {
        std::cout << "test OK" << std::endl;
    }
    else
    {
        std::cout << "test Failed" << std::endl;
    } 
    video_mgr->Test();
    //std::thread t1(SendVideo);

    while (true)
    {
        switch (key_pad->Scan())
        {
            case NULL_DATA:
		       //std::cout << "NULL_DATA \n"; 
                break;

            case CALL:
                if (sys_mgr->FindUnitNumber(key_pad->BuffCpy()))
                {
                    std::cout << "비디오 이미지 전송!!\n";
                    lcd->Write("세대호출 중..");
                    is_video_event = true;
                }
                else
                {
                    lcd->Write("세대호출 실패..");
                }
                key_pad->BuffCler();
                break;

            case PASSWORD:
                /* 패스워드 비교 로직 */
                if (sys_mgr->PwCompare(key_pad->BuffCpy()))
                {
                    std::cout << "doorOpen !!\n";
                }
                break;
                
             /* Write data to LCD */
            default:		
                lcd->Write(key_pad->BuffCpy());
                break;
        }
        usleep(100000);
    }
    return 0;
}
