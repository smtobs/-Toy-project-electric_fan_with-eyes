#include "buzzer.h"
#include "util.h"

#define BUZZER_PWM_DEV   0
#define BUZZER_LABEL     "buzzer_pwm0"

#define FREQ_1KHZ        1000
#define FREQ_2KHZ        2000
#define FREQ_3KHZ        3000

void OnBuzzerSound(buzzer_obj_t *this);
void RemoveBuzzerObj(buzzer_obj_t *this);

bool CreateBuzzerObj(buzzer_obj_t *this)
{    
     /* Create Pwm Obj */
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

void OnBuzzerSound(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = this->pwm;

    p_pwm->StartFunc(p_pwm);

    p_pwm->GenFreqFunc(p_pwm, FREQ_1KHZ);
	DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, FREQ_2KHZ);
	DELAY_MS(100);

    p_pwm->GenFreqFunc(p_pwm, FREQ_3KHZ);
	DELAY_MS(100);

	buzzerStoptFreq();
}

void RemoveBuzzerObj(buzzer_obj_t *this)
{
    pwm_obj_t *p_pwm = this->pwm;

    p_pwm->RemoveObjFunc(p_pwm);

}