#include "pwm.h"
#include <stdbool.h>

typedef struct buzzer_obj_t
{
     pwm_obj_t pwm;
     void (*OnSoundFunc)(buzzer_obj_t *this);
     void (*RemoveObjFunc)(buzzer_obj_t *this);
}buzzer_obj_t;

bool CreateBuzzerObj(buzzer_obj_t *this);