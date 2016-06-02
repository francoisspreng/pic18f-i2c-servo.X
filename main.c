#include <xc.h>
#include "maitre.h"
#include "esclave.h"
#include "pwm.h"
#include "i2c.h"
#include "test.h"

/**
 * Bits de configuration:
 */
#pragma config FOSC = INTIO67   // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF       // Pas d'osc. au démarrage.
#pragma config FCMEN = OFF      // Pas de monitorage de l'oscillateur.

// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR  // RE3 est actif comme master reset.
#pragma config WDTEN = OFF      // Watchdog inactif.
#pragma config LVP = OFF        // Single Supply Enable bits off.

// Sorties des PWM:
#pragma config CCP3MX = PORTC6  // Sortie PA3 connectée sur PORTC6.

#ifndef TEST

typedef enum {
    MODE_MAITRE,
    MODE_ESCLAVE
} Mode;

Mode mode;

/**
 * Point d'entrée des interruptions basse priorité.
 */
void low_priority interrupt interruptionsBassePriorite() {
    if (mode == MODE_MAITRE) {
        maitreInterruptions();
    } else {
        esclaveInterruptions();
    }    
}

/**
 * Point d'entrée.
 * Suivant la valeur du port B4, il lance le programme
 * en mode émetteur ou en mode récepteur.
 */
void main(void) {
    
    TRISBbits.RB4 = 1;
    ANSELBbits.ANSB4 = 0;
    
    if (PORTBbits.RB4) {
        mode = MODE_ESCLAVE;
    } else {
        mode = MODE_MAITRE;
    }
    
    if (mode == MODE_MAITRE) {
        maitreMain();
    } else {
        esclaveMain();
    }
    
    while(1);
}
#endif

#ifdef TEST
void main() {
    initialiseTests();
    testPwm();
    finaliseTests();
    while(1);
}
#endif
