#define sei() __asm__ __volatile__ ("sei" ::: "memory")
#define cli() __asm__ __volatile__ ("cli":::"memory")

#define sei() __asm__ __volatile__ ("sei" ::: "memory")
#define cli() __asm__ __volatile__ ("cli":::"memory")

/*ISR_TIMER0_OVF*/
#define ISR_TIMER0_OVF()\
void __vector_11(void) __attribute__ ((interrupt));\
void __vector_11(void)

/*ISR_TIMER0_COMP*/
#define ISR_TIMER0_COMP()\
void __vector_10(void) __attribute__ ((interrupt));\
void __vector_10(void)

/*ISR_TIMER2_OVF*/   
#define ISR_TIMER2_OVF()\
void __vector_5(void) __attribute__ ((interrupt));\
void __vector_5(void)

/*ISR_INT2*/
#define ISR_INT2()\
void __vector_3(void) __attribute__ ((interrupt));\
void __vector_3(void)

/*ISR_USART_RX_COMPLETE*/
#define ISR_USART_RX()\
void __vector_13(void) __attribute__ ((interrupt));\
void __vector_13(void)

/*ISR_USART_DATA_REGISTER_EMPTY*/
#define ISR_USART_UDRE()\
void __vector_14(void) __attribute__ ((interrupt));\
void __vector_14(void)
