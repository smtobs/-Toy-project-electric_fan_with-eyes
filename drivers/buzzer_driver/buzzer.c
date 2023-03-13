#include "buzzer.h"

#define BUZZER_PWM_DEV   0
#define BUZZER_LABEL     "buzzer_pwm0"

static void SuccessSound(buzzer_obj_t *this);
static void ButtonPushSound(buzzer_obj_t *this);
static void RemoveBuzzerObj(buzzer_obj_t *this);

bool CreateBuzzerObj(struct buzzer_obj_t *this)
{    
    if (this->instance == true)
    {
        printk("The Buzzer Device Driver is already open.");
        return false;
    }

     /* Create PWM Obj */
     if (CreatePwmObj(&this->pwm, BUZZER_PWM_DEV, BUZZER_LABEL) == false)
     {
        return false;
     }

     /* Register for Buzzer Function */
     this->SuccessSoundFunc     = SuccessSound;
     this->ButtonPushSoundFunc  = ButtonPushSound;
     this->RemoveObjFunc        = RemoveBuzzerObj;

    this->instance = true;
     return true;
}

static void SuccessSound(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = &this->pwm;

    p_pwm->StartFunc(p_pwm);

    p_pwm->GenFreqFunc(p_pwm, 50, 1000); //Duty 50%, 1KHz
    U_DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, 50, 2000); //Duty 50%, 2KHz
    U_DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, 50, 3000); //Duty 50%, 3KHz
    U_DELAY_MS(100);

    p_pwm->StoptFunc(p_pwm);
}

static void ButtonPushSound(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = &this->pwm;

    p_pwm->StartFunc(p_pwm);

    p_pwm->GenFreqFunc(p_pwm, 50, 1046);
    U_DELAY_MS(50);

    p_pwm->StoptFunc(p_pwm);
}

static void RemoveBuzzerObj(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = &this->pwm;

    p_pwm->RemoveObjFunc(p_pwm);

    this->instance = false;
}
