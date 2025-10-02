extern volatile int txend;
void uart_init(void);
void uart_putchar(char ch);
void uart_putstr(char *sp);
int uart_putchar_1(char ch,FILE *stream);
int uart_putchar_2(char ch,FILE *stream);
int is_prime(int n);
void uart_putstart(char str[]);
