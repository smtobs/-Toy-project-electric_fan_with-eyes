#include "buzzer.h"
#include "util.h"

#define BUZZER_PWM_DEV   0
#define BUZZER_LABEL     "buzzer_pwm0"

#define FREQ_1KHZ        1000
#define FREQ_2KHZ        2000
#define FREQ_3KHZ        3000

static void OnBuzzerSound(buzzer_obj_t *this);
static void RemoveBuzzerObj(buzzer_obj_t *this);

_Bool CreateBuzzerObj(struct buzzer_obj_t *this)
{    
     /* Create PWM Obj */
     if (CreatePwmObj(&this->pwm, BUZZER_PWM_DEV, BUZZER_LABEL) == false)
     {
        printk("Error CreatePwmObj !\n");
        return false;
     }

     /* Register Buzzer Function */
     this->OnSoundFunc   = OnBuzzerSound;
     this->RemoveObjFunc = RemoveBuzzerObj;
     
     return true;
}

static void OnBuzzerSound(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = &this->pwm;

    p_pwm->StartFunc(p_pwm);

    p_pwm->GenFreqFunc(p_pwm, FREQ_1KHZ);
    U_DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, FREQ_2KHZ);
    U_DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, FREQ_3KHZ);
    U_DELAY_MS(100);
    
    p_pwm->StoptFunc(p_pwm);
}

static void RemoveBuzzerObj(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = &this->pwm;

    p_pwm->RemoveObjFunc(p_pwm);

}
