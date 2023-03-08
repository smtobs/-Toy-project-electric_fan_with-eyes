#include "pwm.h"
#include "util.h"
#include <linux/pwm.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

static void StartPwm(pwm_obj_t *);
static void StopPwm(pwm_obj_t *);
static void GenFreq(pwm_obj_t *, uint16_t, uint16_t);
static void RemovePwmObj(pwm_obj_t *);

typedef struct pwm_private_t
{
    struct pwm_device *instance;
}pwm_private_t;

bool CreatePwmObj(struct pwm_obj_t *pwm,  int pwm, const char *label)
{
     pwm->private = U_MALLOC(sizeof(pwm_private_t));
     
     pwm->private->instance = pwm_request(pwm, label);
     if (!(pwm->private->instance))
     {
          printk("PWM request failed !");
          return false;
     }

     /* Register for PWM Function */
     pwm->StartFunc     = StartPwm;
     pwm->StoptFunc     = StopPwm;
     pwm->GenFreqFunc   = GenFreq;
     pwm->RemoveObjFunc = RemovePwmObj;

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

static void GenFreq(pwm_obj_t *this, uint16_t ratio, uint16_t freq)
{
     uint32_t period, duty;

     period = 1000000000 / freq;
     duty   = (ratio * period) / 100;

     pwm_config(this->private->instance, duty, period);
}
