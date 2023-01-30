#ifndef _PWM_H_
#define _PWM_H_

#include <stdbool.h>

struct pwm_private_t;

typedef struct pwm_obj_t
{
     struct pwm_private_t private;
     void (*StartFunc)(pwm_obj_t *this);
     void (*StoptFunc)(pwm_obj_t *this);
     void (*GenFreqFunc)(pwm_obj_t *this, uint16_t freq);
     void (*RemoveObjFunc)(pwm_obj_t *this);
}pwm_obj_t;

bool CreatePwmObj(pwm_obj_t *this,  int pwm, const char *label);
#endif