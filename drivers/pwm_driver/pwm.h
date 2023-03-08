#ifndef _PWM_H_
#define _PWM_H_

struct pwm_private_t;

typedef struct pwm_obj_t
{
     struct pwm_private_t *private;
     void (*StartFunc)(struct pwm_obj_t *this);
     void (*StoptFunc)(struct pwm_obj_t *this);
     void (*GenFreqFunc)(struct pwm_obj_t *this, unsigned short ratio, unsigned short freq);
     void (*RemoveObjFunc)(struct pwm_obj_t *this);
}pwm_obj_t;

bool CreatePwmObj(struct pwm_obj_t *this,  int pwm, const char *label);
#endif