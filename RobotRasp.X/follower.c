
#include "gpio.h"
#include "board.h"
#include "data.h"

void follower_init(void) {

    
    gpio_set_output(LF1_EN);
    gpio_set_high(LF1_EN);
    gpio_set_analog(LF1_VALUE);

    gpio_set_output(LF2_EN);
    gpio_set_high(LF2_EN);
    gpio_set_analog(LF2_VALUE);

    gpio_set_output(LF3_EN);
    gpio_set_high(LF3_EN);
    gpio_set_analog(LF3_VALUE);

    gpio_set_output(LF4_EN);
    gpio_set_high(LF4_EN);
    gpio_set_analog(LF4_VALUE);

    gpio_set_output(LF5_EN);
    gpio_set_high(LF5_EN);
    gpio_set_analog(LF5_VALUE);


}

void follower_update(void) {
    static int last_dir = 0;
    const int stop = 0;
    const int up = 1;
    const int right = 3;
    const int left = 4;

    float value1, value2, value3, value4, value5;

    value1 = gpio_analog_read(LF1_VALUE);
    value2 = gpio_analog_read(LF2_VALUE);
    value3 = gpio_analog_read(LF3_VALUE);
    value4 = gpio_analog_read(LF4_VALUE);
    value5 = gpio_analog_read(LF5_VALUE);

    data_set(LF_VAL1, value1);
    data_set(LF_VAL2, value2);
    data_set(LF_VAL3, value3);
    data_set(LF_VAL4, value4);
    data_set(LF_VAL5, value5);


    if(data_get(STOP_TO_LINE)){
        if(value3 < 1.5){
            // Le capteur centrale est sur la ligne
            // On stop

            data_set(REG_EN_M1,0);
            data_set(REG_EN_M2,0);

            data_set(REG_M1_DONE, 1);
            data_set(REG_M2_DONE, 1);
        }
    }

    if(data_get(LINE_FOLLOW)){


        if((value1 < 1.5) && (value5 < 1.5)) {
            // Line à droite et à gauche
            // On va tous droit
            if(last_dir != up){
                data_set(FUNC_GO_DIR, up);
                data_set(FUNC_GO, 1);

                last_dir = up;
            }

        }
        else if(value1 < 1.5) {
            // Line noir à droite
            // On tourne à droite

            if(last_dir != right){
                data_set(FUNC_GO_DIR, right);
                data_set(FUNC_GO, 1);
                last_dir = right;
            }

        }
        else if(value5 < 1.5) {
            // Line noir à gauche
            // On tourne à gauche
            if(last_dir != left){
                data_set(FUNC_GO_DIR, left);
                data_set(FUNC_GO, 1);
                last_dir = left;
            }
        }
        else if(value3 < 1.5){
            // Line noir au milieu -> OK
            //On va tout droit
            if(last_dir != up){
                data_set(FUNC_GO_DIR, up);
                data_set(FUNC_GO, 1);
                last_dir = up;
            }
        }
        else {
            // On s'arrete
            //if(last_dir != stop){
            //    data_set(FUNC_GO_DIR, stop);
            //    data_set(FUNC_GO, 1);
            //    last_dir = stop;
            //}
        }

    }

}
