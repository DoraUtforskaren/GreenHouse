#ifndef  _Led_H
#define  _Led_H

#define AT91C_PIOD_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0)
#define AT91C_PIO_IFER (AT91_CAST(AT91_REG *) 0x400E1420)
#define AT91C_PIO_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484)
#define AT91C_PIO_SCDR (AT91_CAST(AT91_REG *) 0x400E148C)

void Set_Led(int nLed);
void readButton(unsigned int *nButton);
void ledconfig();


#endif