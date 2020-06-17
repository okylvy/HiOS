// The functions defined in naskfunc.nas.
void io_hlt(void);
void io_cli(void);
void io_out(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

// The functions defined here.
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);

#define COL8_000000 0    // Black
#define COL8_FF0000 1    // Red
#define COL8_00FF00 2    // Green
#define COL8_FFFF00 3    // Yellow
#define COL8_0000FF 4    // Blue
#define COL8_FF00FF 5    // Pink
#define COL8_00FFFF 6    // Light blue
#define COL8_FFFFFF 7    // White
#define COL8_C6C6C6 8    // Grey
#define COL8_840000 9    // Brown
#define COL8_008400 10   // Deep green
#define COL8_848400 11   // Ocher
#define COL8_000084 12   // Deep blue
#define COL8_840084 13   // Purple
#define COL8_008484 14   // Windows desctop
#define COL8_848484 15   // Grey

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

void HiosMain(void) {
    char *vram;
    int xsize, ysize;
    struct BOOTINFO *binfo;
    static char font_A[16] = {
        0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
        0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00,
    };

    init_palette();

    binfo = (struct BOOTINFO *) 0x0ff0;  // 0x0ff0 is defined in nasmhead.asm as boot info.
    xsize = binfo->scrnx;
    ysize = binfo->scrny;
    vram = binfo->vram;

    init_screen(vram, xsize, ysize);
    putfont8(vram, xsize, 10, 15, COL8_000000, font_A);

/* Draw stripe by assigning specific value in VRAM address.
    int i;
    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;
    }
*/

    for (;;) {
        io_hlt();
    }
}

void init_palette(void) {
    static unsigned char table_rgb[16 * 3] = {
        // The num of elements is 16 x 3.
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0xff, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb) {
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();

    io_out8(0x03c8, start);  // 0x03c8: addr register
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);  // 0x03c9: data register
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
    return;
}

void init_screen(char *vram, int x, int y) {
    // Desctop screen
    boxfill8(vram,
             x,
             COL8_C6C6C6,
             0,
             0,
             x - 1,
             y - 1);

    // Top bar
    boxfill8(vram,
             x,
             COL8_848484,
             0,
             0,
             x - 1,
             10);

    // Small icon on top bar
    boxfill8(vram,
             x,
             COL8_FF0000,
             2,
             2,
             8,
             8);

    // Tool bar below
    boxfill8(vram,
             x,
             COL8_848484,
             50,
             y - 28,
             x - 50,
             y - 1);

    return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font) {
    int i;
    char *p, d;
    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0x80) != 0) { p[0] = c; }  // 1st column - 10000000
        if ((d & 0x40) != 0) { p[1] = c; }  // 2nd column - 01000000
        if ((d & 0x20) != 0) { p[2] = c; }  // 3rd column - 00100000
        if ((d & 0x10) != 0) { p[3] = c; }  // 4th column - 00010000
        if ((d & 0x08) != 0) { p[4] = c; }  // 5th column - 00001000
        if ((d & 0x04) != 0) { p[5] = c; }  // 6th column - 00000100
        if ((d & 0x02) != 0) { p[6] = c; }  // 7th column - 00000010
        if ((d & 0x01) != 0) { p[7] = c; }  // 8th column - 00000001
    }
}
