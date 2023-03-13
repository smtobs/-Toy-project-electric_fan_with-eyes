#include <iostream>
#include <thread>
#include "unistd.h"
#include "oled.hpp"
#include "keyPad.hpp"
#include "systemManager.hpp"
#include "buzzer.hpp"

#define PASSWORD       'A'
#define CALL           'B'
#define CLEAR          'C'
#define NULL_DATA      0

bool is_video_event;

int main()
{
    std::vector<char> test1 = {'1', '2', '3', '4'};
    Buzzer* buzzer = new Buzzer();
    KeyPad* key_pad = new KeyPad();
    Oled* oled = new Oled();
    SystemManager *sys_mgr = new SystemManager();

    oled->ClearDisplay();
    oled->WriteDisplay("Please enter your\n", 2, 17);
    oled->WriteDisplay("password.\n", 3, 40);

    uint8_t curs_pos_count = 0;

    oled->WriteDisplay(sys_mgr->GetLocalTime(), 0, 0);
    
    char keypad_data[2] = {0, };

    while (true)
    {
        keypad_data[0] = key_pad->Scan();
        switch (keypad_data[0])
        {
            case NULL_DATA:
                break;

            case CALL:
#if (0)
                if (sys_mgr->FindUnitNumber(key_pad->BuffCpy()))
                {
                    std::cout << "비디오 이미지 전송!!\n";
                    oled->WriteDisplay("세대호출 중..");
                    is_video_event = true;
                }
                else
                {
                    oled->WriteDisplay("세대호출 실패..");
                }
#endif
                key_pad->BuffCler();
                break;

            case PASSWORD:
                if (sys_mgr->PwCompare(key_pad->BuffCpy()))
                {
                    oled->ClearDisplay();                 
                    oled->WriteDisplay("Correct Password.\n", 3, 14);
                    oled->WriteDisplay("The door opens.\n", 4, 20);
                    buzzer->SuccessSound();
                }
                break;

            case CLEAR:
                oled->ClearDisplay();
                key_pad->BuffCler();
                oled->WriteDisplay("Please enter your\n", 2, 17);
                oled->WriteDisplay("password.\n", 3, 40);
                buzzer->ButtonPushSound();
                break;

            default: /* Write data to LCD */
                if (curs_pos_count < 5)
                {
                    if (curs_pos_count == 0)
                    {
                        oled->WriteDisplay(keypad_data, 5, 45 + (curs_pos_count * 8));
                    }
                    else
                    {
                        oled->WriteDisplay("*", 5, 45 + ((curs_pos_count - 1) * 8));
                        oled->WriteDisplay(keypad_data, 5, 45 + (curs_pos_count * 8));
                    }
                    curs_pos_count++;

                    buzzer->ButtonPushSound();
                }
                break;
        }
        oled->WriteDisplay(sys_mgr->GetLocalTime(), 0, 0);
        usleep(1000);
    }
    return 0;
}
