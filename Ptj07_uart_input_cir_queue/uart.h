extern volatile int txend;
void uart_init(void);
void uart_putstr(char *sp);
int is_prime(int n);
void uart_putstart(char str[]);
int uart_putchar(char ch, FILE *stream);
void app_prime(int t);
