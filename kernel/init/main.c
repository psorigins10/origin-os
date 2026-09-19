void kernel_main(void)
{
  const char *message = "Welcome to ORIGIN OS\n";

    for (int i = 0; message[i] != '\0'; i++) {
        __asm__ volatile (
            "mov $0x3f8, %%dx\n"
            "out %%al, %%dx\n"
            :
            : "a"(message[i])
            : "rdx"
        );
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}