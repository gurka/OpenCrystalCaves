#!/usr/bin/env python

import sys
from PIL import Image

def can_read(f):
    b = f.read(1)
    if len(b) == 0:
        return False
    f.seek(-1, 1)
    return True


def read_u8(f):
    return ord(f.read(1))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: {} FILE".format(sys.argv[0]))
        sys.exit(1)

    # Go through the file once and figure out how many sprites in total there are
    num_sprites_total = 0
    with open(sys.argv[1], 'rb') as f:
        while can_read(f):
            num_sprites = read_u8(f)
            width_bytes = read_u8(f)
            height_pixels = read_u8(f)

            num_sprites_total += num_sprites

            f.read(num_sprites * width_bytes * height_pixels * 5)

    print("Found a total of {} sprites.".format(num_sprites_total))

    # For Crystal Caves 1 (CC1.GFX) there should be 1150 sprites
    if num_sprites_total != 1150:
        print("This script only supports Crystal Caves 1 GFX (CC1.GFX).")
        sys.exit(1)

    # Let's make the spritesheet 544 x 544 pixels (34 x 34 = 1156 sprites)
    img = Image.new('RGBA', (544, 544), 'black')
    pixels = img.load()

    # Make the image fully transparent at start
    for y in range(544):
        for x in range(544):
            pixels[x, y] = (0, 0, 0, 0)

    # Open the file again and draw each sprite to 'pixels'
    with open(sys.argv[1], 'rb') as f:
        # The number of the current sprite we're drawing (0..1150)
        # Used to figure out where it should be drawn (x, y)
        sprite_num = 0

        while can_read(f):
            num_sprites = read_u8(f)
            width_bytes = read_u8(f)
            height_pixels = read_u8(f)

            for sprite in range(num_sprites):
                # This is where this sprite should be drawn
                x_start = (sprite_num % 34) * 16
                y_start = (sprite_num / 34) * 16
                x = x_start
                y = y_start

                for h in range(height_pixels):
                    for w in range(width_bytes):
                        t_plane = read_u8(f)
                        b_plane = read_u8(f)
                        g_plane = read_u8(f)
                        r_plane = read_u8(f)
                        i_plane = read_u8(f)

                        for pixel in range(8):
                            bit = 7 - pixel
                            t = ((t_plane >> bit) & 1)
                            b = ((b_plane >> bit) & 1)
                            g = ((g_plane >> bit) & 1)
                            r = ((r_plane >> bit) & 1)
                            i = ((i_plane >> bit) & 1)

                            if t == 0:
                                pixels[x, y] = (0, 0, 0, 0)

                            else:
                                re = 0 if r == 0 else 170
                                gr = 0 if g == 0 else 170
                                bl = 0 if b == 0 else 170

                                if i == 1:
                                    re += 85
                                    gr += 85
                                    bl += 85

                                pixels[x, y] = (re, gr, bl, 255)

                            x += 1
                            if x == x_start + 16:
                                y += 1
                                x = x_start

                sprite_num += 1

    img.save('sprites.png')
    print("Wrote 'sprites.png'.")
