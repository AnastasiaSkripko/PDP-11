#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;         // 8 bit
typedef unsigned short int word;    // 16 bit
typedef word address;               // 16 bit

#define MEMSIZE (64*1024)           // 64 kb

byte mem[MEMSIZE];                  // RAM

void b_write(address adr, byte b) {
    mem[adr] = b;
}
byte b_read(address adr) {
    return mem[adr];
}
void w_write(address adr, word w) {
    mem[adr] = w & 0xFF;            // младшие 8 бит
    mem[adr+1] = (w >> 8) & 0xFF;   // старшие 8 бит
}
word w_read(address adr) {
    word w = ((word)mem[adr+1]) << 8;
    w = w | mem[adr];
    return w & 0xFFFF;
}

void test_mem() {
    address a;
    byte b0, b1, bres;
    word w, wres;

    // пишем байт, читаем байт
    fprintf(stderr, "Пишем и читаем байт по четному адресу\n");
    a = 0;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%04x b0=%02hhx bres=%02hhx\n", a, b0, bres);
    assert(b0 == bres);

    fprintf(stderr, "Пишем и читаем байт по нечетному адресу\n");
    a = 1;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%04x b0=%02hhx bres=%02hhx\n", a, b0, bres);
    assert(b0 == bres);

    // пишем слово, читаем слово
    fprintf(stderr, "Пишем и читаем слово\n");
    a = 2;
    w = 0xb0b1;
    w_write(a, w);
    wres = w_read(a);
    fprintf(stderr, "a=%04x w=%04x wres=%04x\n", a, w, wres);
    assert(w == wres);

    // пишем 2 байта, читаем 1 слово
    fprintf(stderr, "Пишем 2 байта, читаем слово\n");
    a = 4;
    w = 0xa1a2;
    // little-endian
    b0 = 0xa2;
    b1 = 0xa1;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    fprintf(stderr, "a=%04x b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);

    // пишем 1 слово, читаем байты
    fprintf(stderr, "Пишем 1 слово, читаем 2 байта\n");
    a = 6;
    w = 0xb1b2;
    w_write(a, w);
    byte bres0 = b_read(a);
    byte bres1 = b_read(a+1);
    fprintf(stderr, "a=%04x b1=%02hhx b0=%02hhx w=%04x\n", a, bres1, bres0, w);
    assert(((bres1 << 8) | bres0) == w);
}

void load_data() {
    address adr;         // адрес начала блока
    unsigned int N;      // количество байт в блоке
    byte x;
    while (scanf("%hx %x", &adr, &N) == 2)  
        for (unsigned int i = 0; i < N; i++) {
            scanf("%hhx", &x);
            if (adr + i < MEMSIZE)
                b_write(adr + i, x);
        }

}

void mem_dump(address adr, int size) {
    for(int i = 0; i < size; i += 2) {
        printf("%06o: %06o %04x\n", adr + i, w_read(adr + i), w_read(adr + i));
    }
}

int main() {
    //test_mem();
    load_data();

    mem_dump(0x40, 20);
    printf("\n");
    mem_dump(0x200, 0x26);

    return 0;
}