#define sei() __asm__ __volatile__ ("sei" ::: "memory")
#define cli() __asm__ __volatile__ ("cli":::"memory")

#define ISR_TIMER0_OVF() \
   void __vector_11(void) __attribute__ ((interrupt));\
   void __vector_11(void)

#define ISR_TIMER0_COMP() \
   void __vector_10(void) __attribute__ ((interrupt));\
   void __vector_10(void) 

/*
void TIMER0_COMP_vect(void) __attribute__ ((interrupt));
void __attribute__ ((interrupt)) T1_OV_ISR(void);
void __attribute__ ((interrupt)) T1_OCA_ISR(void);
void __attribute__ ((interrupt)) T1_OCB_ISR(void);
void __attribute__ ((interrupt)) T1_IC_ISR(void);
void __attribute__ ((interrupt)) T2_OV_ISR(void);
void __attribute__ ((interrupt)) T2_OC_ISR(void);
*/