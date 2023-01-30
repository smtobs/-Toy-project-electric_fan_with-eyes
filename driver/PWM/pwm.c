#include "pwm.h"
#include <linux/pwm.h>

static void StartPwm(pwm_obj *);
static void StopPwm(pwm_obj *);
static void GenFreq(pwm_obj *, uint16_t);
static void RemovePwmObj(pwm_obj *);

typedef struct pwm_private_t
{
    struct pwm_device instance;
}pwm_private_t;

bool CreatePwmObj(pwm_obj_t *this,  int pwm, const char *label)
{
     //pwm_private_t pwm_private;

     this->private.instance = pwm_request(pwm, label);
     if (!this->private.instance)
     {
        // error
        return false;
     }

     /* Register PWM Function */
     this->StartFunc     = StartPwm;
     this->StoptFunc     = StopPwm;
     this->GenFreqFunc   = GenFreq;
     this->RemoveObjFunc = RemovePwmObj;

     //memcpy(this->private, pwm_private, sizeof(pwm_private));

     return true;
}

static void RemovePwmObj(pwm_obj_t *this)
{
     /* Disable PWM & Free PWM GPIO */
     if (this->private.instanse)
     {
        this->StoptFunc(this);
        pwm_free(this->private.instanse);
     }
}

static void StartPwm(pwm_obj_t *this)
{
     pwm_enable(this->private.instance);
}

static void StopPwm(pwm_obj_t *this)
{
     pwm_disable(this->private.instance);
}

static void GenFreq(pwm_obj_t *this, uint16_t freq)
{
     uint16_t duty, period;
    
     period = (1000000000 / freq) - 1; 
     duty = period / 2;

     pwm_config(this->private.instance, duty, period);
}
