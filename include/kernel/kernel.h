struct CpuRegisters {
    int eax;
    int ebx;
    int ecx;
    int edx;
};

#ifdef __cplusplus
extern "C" {
#endif

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);

#ifdef __cplusplus
}
#endif
