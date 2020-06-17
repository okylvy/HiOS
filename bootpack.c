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

void HiosMain(void) {
    init_palette();

    char *p;
    p = (char *)0xa0000;
    int xsize = 320;
    int ysize = 200;

/* Draw stripe by assigning specific value in VRAM address.
    int i;
    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;
    }
*/
    // Desctop screen
    boxfill8(p,
             xsize,
             COL8_C6C6C6,
             0,
             0,
             xsize - 1,
             ysize - 1);

    // Top bar
    boxfill8(p,
             xsize,
             COL8_848484,
             0,
             0,
             xsize - 1,
             10);

    // Small icon on top bar
    boxfill8(p,
             xsize,
             COL8_FF0000,
             2,
             2,
             8,
             8);

    // Tool bar below
    boxfill8(p,
             xsize,
             COL8_848484,
             50,
             ysize - 28,
             xsize - 50,
             ysize - 1);

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
