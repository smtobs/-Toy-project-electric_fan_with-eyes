#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "pwm.h"
#include "util.h"

typedef struct buzzer_obj_t
{
     pwm_obj_t pwm;
     int major_num;
     bool instance;
     void (*SuccessSoundFunc)(struct buzzer_obj_t *this);
     void (*FailSoundFunc)(struct buzzer_obj_t *this);
     void (*ButtonPushSoundFunc)(struct buzzer_obj_t *this);
     void (*RemoveObjFunc)(struct buzzer_obj_t *this);
}buzzer_obj_t;

bool CreateBuzzerObj(struct buzzer_obj_t *this);
#endif