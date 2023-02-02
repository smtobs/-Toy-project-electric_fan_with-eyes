#include "pwm.h"
#include "util.h"
#include <linux/pwm.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

static void StartPwm(pwm_obj_t *);
static void StopPwm(pwm_obj_t *);
static void GenFreq(pwm_obj_t *, uint16_t);
static void RemovePwmObj(pwm_obj_t *);

typedef struct pwm_private_t
{
    struct pwm_device *instance;
}pwm_private_t;

_Bool CreatePwmObj(struct pwm_obj_t *this,  int pwm, const char *label)
{
     this->private = U_MALLOC(sizeof(pwm_private_t));
     
     this->private->instance = pwm_request(pwm, label);
     if (!(this->private->instance))
     {
        // Todo
        return false;
     }

     /* Register PWM Function */
     this->StartFunc     = StartPwm;
     this->StoptFunc     = StopPwm;
     this->GenFreqFunc   = GenFreq;
     this->RemoveObjFunc = RemovePwmObj;

     return true;
}

static void RemovePwmObj(pwm_obj_t *this)
{
     /* Disable PWM & Free PWM GPIO */
     if (this->private->instance)
     {
        this->StoptFunc(this);
        pwm_free(this->private->instance);
     }

     U_SAFETY_FREE(this->private);
}

static void StartPwm(pwm_obj_t *this)
{
     pwm_enable(this->private->instance);
}

static void StopPwm(pwm_obj_t *this)
{
     pwm_disable(this->private->instance);
}

static void GenFreq(pwm_obj_t *this, uint16_t freq)
{
     uint16_t duty, period;
    
     period = (1000000000 / freq) - 1; 
     duty = period / 2;

     pwm_config(this->private->instance, duty, period);
}
