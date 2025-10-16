extern volatile int txend;
void uart_init(void);
void uart_putstr(char *sp);
int uart_putchar_1(char ch,FILE *stream);
int uart_putchar_2(char ch,FILE *stream);
int is_prime(int n);
void uart_putstart(char str[]);
int uart_putchar_6(char ch, FILE *stream);
void app_prime(int t);
