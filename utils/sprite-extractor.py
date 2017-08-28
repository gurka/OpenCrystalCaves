#!/usr/bin/env python

import sys
from PIL import Image

# irgb -> RGBA
# basically:
# if r set => r +170
# if g set => g +170
# if b set => b +170
# if i set => all +85
# one special case
PALETTE = [        # irgb
    (  0,   0,   0, 255),  # 0000
    (  0,   0, 170, 255),  # 0001
    (  0, 170,   0, 255),  # 0010
    (  0, 170, 170, 255),  # 0011
    (170,   0,   0, 255),  # 0100
    (170,   0, 170, 255),  # 0101
    (170,  85,   0, 255),  # 0110 - special
    (170, 170, 170, 255),  # 0111
    ( 85,  85,  85, 255),  # 1000
    ( 85,  85, 255, 255),  # 1001
    ( 85, 255,  85, 255),  # 1010
    ( 85, 255, 255, 255),  # 1011
    (255,  85,  85, 255),  # 1100
    (255,  85, 255, 255),  # 1101
    (255, 255,  85, 255),  # 1110
    (255, 255, 255, 255),  # 1111
]

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

    # CC1.GFX contains 23 tilesets with 50 sprites each: 23 * 50 = 1150 sprites
    # We want to extract and draw them onto an image with 4 sprites per row
    # We also want to make sure that each tileset starts on a separate row
    # Each tileset will result in 4 x 13 = 52 sprites (2 filler sprites), occupying
    # 64 x 208 pixels
    # The whole image will be 64 x 4784 pixels and 4 x 13 x 23 = 1196 sprites
    # (46 filler sprites)
    IMAGE_WIDTH = 64
    IMAGE_HEIGHT = 4784

    img = Image.new('RGBA', (IMAGE_WIDTH, IMAGE_HEIGHT), 'black')
    pixels = img.load()

    # Make the image fully transparent at start
    for y in range(IMAGE_HEIGHT):
        for x in range(IMAGE_WIDTH):
            pixels[x, y] = (0, 0, 0, 0)

    with open(sys.argv[1], 'rb') as f:
        # The number of the current sprite we're drawing (0..1196)
        # Used to figure out where it should be drawn (x, y)
        sprite_num = 0

        while can_read(f):
            num_sprites = read_u8(f)
            width_bytes = read_u8(f)
            height_pixels = read_u8(f)

            for sprite in range(num_sprites):
                # This is where this sprite should be drawn
                x_start = (sprite_num % 4) * 16
                y_start = (sprite_num / 4) * 16
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
                                pixels[x, y]  = PALETTE[(i << 3) | (r << 2) | (g << 1) | b]

                            x += 1
                            if x == x_start + 16:
                                y += 1
                                x = x_start

                sprite_num += 1

            # Here is where we need to add two filler sprites
            # (sprite_num % 50 == 0)
            sprite_num += 2

    img.save('sprites.png')
    print("Wrote 'sprites.png'.")
