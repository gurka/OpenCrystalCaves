#pragma once

enum class Sprite : int
{
  SPRITE_NONE = -1,
  SPRITE_CANDLE = 3,
  SPRITE_CLEAR_BLOCK = 6,
  SPRITE_PICKAXE = 8,
  SPRITE_SHOVEL = 10,
  SPRITE_ENTRY_1 = 12,
  SPRITE_ENTRY_2 = 13,
  SPRITE_ENTRY_3 = 14,
  SPRITE_ENTRY_DONE = 15,
  SPRITE_ROCKS_1 = 20,
  SPRITE_ROCKS_2 = 21,
  SPRITE_ROCKS_3 = 24,
  SPRITE_ROCKS_4 = 25,
  SPRITE_ROCKMAN_L_1 = 104,
  SPRITE_ROCKMAN_L_2 = 105,
  SPRITE_ROCKMAN_L_3 = 106,
  SPRITE_ROCKMAN_L_4 = 107,
  SPRITE_ROCKMAN_L_5 = 108,
  SPRITE_ROCKMAN_WALK_L_1 = 109,
  SPRITE_ROCKMAN_WALK_L_2 = 110,
  SPRITE_ROCKMAN_WALK_L_3 = 111,
  SPRITE_ROCKMAN_WALK_L_4 = 112,
  SPRITE_ROCKMAN_WALK_L_5 = 113,
  SPRITE_ROCKMAN_WALK_L_6 = 114,
  SPRITE_ROCKMAN_WALK_L_7 = 115,
  SPRITE_SPIDER_DOWN_1 = 128,
  SPRITE_SPIDER_UP_1 = 136,
  SPRITE_RED_CRATE_1 = 144,
  SPRITE_RED_CRATE_2 = 145,
  SPRITE_SNAKE_WALK_L_1 = 160,
  SPRITE_SNAKE_PAUSE_1 = 169,
  SPRITE_SNAKE_WALK_R_1 = 176,
  SPRITE_HORIZON_LAMP = 240,
  SPRITE_THORN_1 = 241,
  SPRITE_HORIZON_1 = 247,
  SPRITE_HORIZON_2 = 248,
  SPRITE_HORIZON_3 = 249,
  SPRITE_HORIZON_4 = 250,
  SPRITE_MINE_SIGN_1 = 251,
  SPRITE_MINE_SIGN_2 = 252,
  SPRITE_STANDING_RIGHT = 260,
  SPRITE_PISTOL = 308,
  SPRITE_POWER = 309,
  SPRITE_STARS_1 = 356,
  SPRITE_STARS_2 = 357,
  SPRITE_STARS_3 = 358,
  SPRITE_STARS_4 = 359,
  SPRITE_STARS_5 = 360,
  SPRITE_STARS_6 = 361,
  SPRITE_PIPE_V = 408,
  SPRITE_PIPE_H = 409,
  SPRITE_PIPE_UR = 410,
  SPRITE_PIPE_UL = 411,
  SPRITE_PIPE_DL = 412,
  SPRITE_PIPE_DR = 413,
  SPRITE_LIGHT_SWITCH_OFF = 429,
  SPRITE_LIGHT_SWITCH_ON = 430,
  SPRITE_LEVER_R_OFF = 432,
  SPRITE_LEVER_R_ON = 433,
  SPRITE_LEVER_B_OFF = 434,
  SPRITE_LEVER_B_ON = 435,
  SPRITE_LEVER_G_OFF = 436,
  SPRITE_LEVER_G_ON = 437,
  SPRITE_DOOR_CLOSED_R_1 = 440,
  SPRITE_DOOR_CLOSED_B_1 = 441,
  SPRITE_DOOR_CLOSED_G_1 = 442,
  SPRITE_DOOR_CLOSED_R_2 = 444,
  SPRITE_DOOR_CLOSED_B_2 = 445,
  SPRITE_DOOR_CLOSED_G_2 = 446,
  SPRITE_SIGN_DOWN = 458,
  SPRITE_SIGN_UP = 459,
  SPRITE_WINNERS_1 = 468,
  SPRITE_WINNERS_2 = 469,
  SPRITE_WINNERS_3 = 470,
  SPRITE_REVERSE_GRAVITY_1 = 472,
  SPRITE_REVERSE_GRAVITY_2 = 473,
  SPRITE_LOW_GRAVITY_1 = 474,
  SPRITE_LOW_GRAVITY_2 = 475,
  SPRITE_DANGER_1 = 476,
  SPRITE_DANGER_2 = 477,
  SPRITE_HOPPER_1 = 496,
  SPRITE_RED_SCAFFOLD_1 = 520,
  SPRITE_RED_SCAFFOLD_2 = 521,
  SPRITE_RED_SCAFFOLD_3 = 522,
  SPRITE_RED_SCAFFOLD_4 = 523,
  SPRITE_RED_SCAFFOLD_5 = 524,
  SPRITE_RED_SCAFFOLD_6 = 525,
  SPRITE_RED_SCAFFOLD_7 = 526,
  SPRITE_RED_SCAFFOLD_8 = 527,
  SPRITE_GREY_STONE_1 = 528,
  SPRITE_GREY_STONE_2 = 529,
  SPRITE_GREY_STONE_3 = 532,
  SPRITE_GREY_STONE_4 = 533,
  SPRITE_DIAMOND_WALL_1 = 548,
  SPRITE_DIAMOND_WALL_2 = 549,
  SPRITE_COBBLE_1 = 550,
  SPRITE_COBBLE_2 = 551,
  SPRITE_DIAMOND_WALL_3 = 552,
  SPRITE_DIAMOND_WALL_4 = 553,
  SPRITE_COBBLE_3 = 554,
  SPRITE_COBBLE_4 = 555,
  SPRITE_BARREL = 575,
  SPRITE_LASER_R = 581,
  SPRITE_LASER_L = 582,
  SPRITE_BARREL_CRACKED = 583,
  SPRITE_EXIT_TOP_LEFT_1 = 584,
  SPRITE_EXIT_TOP_LEFT_2 = 585,
  SPRITE_EXIT_TOP_LEFT_3 = 586,
  SPRITE_EXIT_TOP_LEFT_4 = 587,
  SPRITE_EXIT_BOTTOM_LEFT_1 = 588,
  SPRITE_EXIT_BOTTOM_LEFT_2 = 589,
  SPRITE_EXIT_BOTTOM_LEFT_3 = 590,
  SPRITE_EXIT_BOTTOM_LEFT_4 = 591,
  SPRITE_EXIT_TOP_RIGHT_1 = 592,
  SPRITE_EXIT_BOTTOM_RIGHT_1 = 596,
  SPRITE_EXIT_BOTTOM_RIGHT_2 = 597,
  SPRITE_EXIT_BOTTOM_RIGHT_3 = 598,
  SPRITE_EXIT_BOTTOM_RIGHT_4 = 599,
  SPRITE_SWITCH_OFF = 602,
  SPRITE_SWITCH_ON = 603,
  SPRITE_WOOD_V = 608,
  SPRITE_GRAVITY = 609,
  SPRITE_PLATFORM_H_1 = 612,
  SPRITE_PLATFORM_H_2 = 613,
  SPRITE_PLATFORM_H_3 = 614,
  SPRITE_PLATFORM_H_4 = 615,
  SPRITE_PLATFORM_V_1 = 616,
  SPRITE_PLATFORM_V_2 = 617,
  SPRITE_PLATFORM_V_3 = 618,
  SPRITE_PLATFORM_V_4 = 619,
  SPRITE_WOOD_H = 620,
  SPRITE_CRYSTAL_1_R = 624,
  SPRITE_CRYSTAL_1_Y = 625,
  SPRITE_CRYSTAL_1_G = 626,
  SPRITE_CRYSTAL_1_B = 627,
  SPRITE_EARTH = 632,
  SPRITE_MOON = 633,
  SPRITE_MOON_SMALL = 634,
  SPRITE_CRYSTAL_2_R = 636,
  SPRITE_CRYSTAL_2_Y = 637,
  SPRITE_CRYSTAL_2_G = 638,
  SPRITE_CRYSTAL_2_B = 639,
  SPRITE_CRYSTAL_3_R = 656,
  SPRITE_CRYSTAL_3_Y = 657,
  SPRITE_CRYSTAL_3_G = 658,
  SPRITE_CRYSTAL_3_B = 659,
  SPRITE_CRYSTAL_HIDDEN = 660,
  SPRITE_TORCH_1 = 663,
  SPRITE_TORCH_2 = 664,
  SPRITE_TORCH_3 = 665,
  SPRITE_TORCH_4 = 666,
  SPRITE_MUSHROOM_RED = 668,
  SPRITE_MUSHROOM_GREEN = 669,
  SPRITE_VOLCANO_TOP_1 = 737,
  SPRITE_VOLCANO_TOP_2 = 738,
  SPRITE_VOLCANO_BOTTOM_1 = 740,
  SPRITE_VOLCANO_BOTTOM_2 = 741,
  SPRITE_VOLCANO_BOTTOM_3 = 742,
  SPRITE_VOLCANO_BOTTOM_4 = 743,
  SPRITE_VOLCANO_EJECTA_L_1 = 748,
  SPRITE_VOLCANO_EJECTA_L_2 = 749,
  SPRITE_VOLCANO_EJECTA_L_3 = 750,
  SPRITE_VOLCANO_EJECTA_L_4 = 751,
  SPRITE_VOLCANO_EJECTA_R_1 = 752,
  SPRITE_VOLCANO_EJECTA_R_2 = 753,
  SPRITE_VOLCANO_EJECTA_R_3 = 754,
  SPRITE_VOLCANO_EJECTA_R_4 = 755,
  SPRITE_GRILLE_1 = 728,
  SPRITE_GRILLE_2 = 729,
  SPRITE_GRILLE_3 = 732,
  SPRITE_GRILLE_4 = 733,
  SPRITE_BRICK_1 = 780,
  SPRITE_BRICK_2 = 781,
  SPRITE_PEBBLE_WALL_1 = 782,
  SPRITE_PEBBLE_WALL_2 = 783,
  SPRITE_BRICK_3 = 784,
  SPRITE_BRICK_4 = 785,
  SPRITE_PEBBLE_WALL_3 = 786,
  SPRITE_PEBBLE_WALL_4 = 787,
  SPRITE_BLOCK_METAL_NW = 788,
  SPRITE_BLOCK_METAL_N = 789,
  SPRITE_BLOCK_METAL_NE = 790,
  SPRITE_BLOCK_METAL_SW = 792,
  SPRITE_BLOCK_METAL_S = 793,
  SPRITE_BLOCK_METAL_SE = 794,
  SPRITE_DARK_STONE_1 = 804,
  SPRITE_DARK_STONE_2 = 805,
  SPRITE_DARK_STONE_3 = 806,
  SPRITE_DARK_STONE_4 = 807,
  SPRITE_DARK_STONE_5 = 808,
  SPRITE_DARK_STONE_6 = 809,
  SPRITE_RED_RECT_1 = 812,
  SPRITE_RED_RECT_2 = 813,
  SPRITE_RED_RECT_3 = 814,
  SPRITE_RED_RECT_4 = 815,
  SPRITE_RED_RECT_5 = 816,
  SPRITE_RED_RECT_6 = 817,
  SPRITE_RED_RECT_7 = 818,
  SPRITE_RED_RECT_8 = 819,
  SPRITE_GREEN_SCAFFOLD_1 = 820,
  SPRITE_GREEN_SCAFFOLD_2 = 821,
  SPRITE_GREEN_SCAFFOLD_3 = 822,
  SPRITE_GREEN_SCAFFOLD_4 = 824,
  SPRITE_GREEN_SCAFFOLD_5 = 825,
  SPRITE_GREEN_SCAFFOLD_6 = 826,
  SPRITE_WOOD_STRUT_1 = 886,
  SPRITE_WOOD_STRUT_2 = 887,
  SPRITE_WOOD_PILLAR_1 = 890,
  SPRITE_WOOD_PILLAR_2 = 891,
  SPRITE_100 = 899,
  SPRITE_SLIME_R_1 = 916,
  SPRITE_SLIME_L_1 = 920,
  SPRITE_SLIME_U_1 = 924,
  SPRITE_SLIME_D_1 = 928,
  SPRITE_HEX_ROCKS_1 = 944,
  SPRITE_HEX_ROCKS_2 = 945,
  SPRITE_HEX_ROCKS_3 = 946,
  SPRITE_HEX_ROCKS_4 = 947,
  SPRITE_HEX_ROCKS_5 = 948,
  SPRITE_HEX_ROCKS_6 = 949,
  SPRITE_HEX_ROCKS_7 = 950,
  SPRITE_HEX_ROCKS_8 = 951,
  SPRITE_WOOD_WALL_1 = 952,
  SPRITE_WOOD_WALL_2 = 953,
  SPRITE_WOOD_WALL_3 = 954,
  SPRITE_WOOD_WALL_4 = 955,
  SPRITE_WOOD_WALL_5 = 956,
  SPRITE_WOOD_WALL_6 = 957,
  SPRITE_WOOD_WALL_7 = 958,
  SPRITE_WOOD_WALL_8 = 959,
  SPRITE_VERTICAL_WALL_1 = 960,
  SPRITE_VERTICAL_WALL_2 = 961,
  SPRITE_METAL_BARS_1 = 962,
  SPRITE_METAL_BARS_2 = 963,
  SPRITE_VERTICAL_WALL_3 = 964,
  SPRITE_VERTICAL_WALL_4 = 965,
  SPRITE_METAL_BARS_3 = 966,
  SPRITE_METAL_BARS_4 = 967,
  SPRITE_BLUE_DIAMOND_1 = 968,
  SPRITE_BLUE_DIAMOND_2 = 969,
  SPRITE_RED_PANEL_1 = 970,
  SPRITE_RED_PANEL_2 = 971,
  SPRITE_BLUE_DIAMOND_3 = 972,
  SPRITE_BLUE_DIAMOND_4 = 973,
  SPRITE_RED_PANEL_3 = 974,
  SPRITE_RED_PANEL_4 = 975,
  SPRITE_COLUMN_AND_KNOB_1 = 976,
  SPRITE_COLUMN_AND_KNOB_2 = 977,
  SPRITE_COLUMN_AND_KNOB_3 = 980,
  SPRITE_COLUMN_AND_KNOB_4 = 981,
  SPRITE_PLATFORM_BLUE = 988,
  SPRITE_PLATFORM_RED = 989,
  SPRITE_PLATFORM_GREEN = 990,
  SPRITE_BUMP_PLATFORM_BLUE_L = 991,
  SPRITE_BUMP_PLATFORM_BLUE_MID = 992,
  SPRITE_BUMP_PLATFORM_BLUE_R = 993,
  SPRITE_BUMP_PLATFORM_RED_L = 994,
  SPRITE_BUMP_PLATFORM_RED_MID = 995,
  SPRITE_BUMP_PLATFORM_RED_R = 996,
  SPRITE_BUMP_PLATFORM_GREEN_L = 997,
  SPRITE_BUMP_PLATFORM_GREEN_MID = 998,
  SPRITE_BUMP_PLATFORM_GREEN_R = 999,
  SPRITE_BLOCK_PEBBLE_NW = 1008,
  SPRITE_BLOCK_PEBBLE_N = 1009,
  SPRITE_BLOCK_PEBBLE_NE = 1010,
  SPRITE_BLOCK_PEBBLE_SW = 1012,
  SPRITE_BLOCK_PEBBLE_S = 1013,
  SPRITE_BLOCK_PEBBLE_SE = 1014,
  SPRITE_BLOCK_PEBBLE_W = 1016,
  SPRITE_BLOCK_PEBBLE_MID = 1017,
  SPRITE_BLOCK_PEBBLE_E = 1018,
  SPRITE_BIGFOOT_HEAD_R_1 = 1020,
  SPRITE_BIGFOOT_BODY_R_1 = 1024,
  SPRITE_BIGFOOT_HEAD_L_1 = 1028,
  SPRITE_BIGFOOT_BODY_L_1 = 1032,
  SPRITE_CONCRETE = 1092,
  SPRITE_CONCRETE_H = 1093,
  SPRITE_CONCRETE_V = 1094,
  SPRITE_CONCRETE_X = 1095,
  SPRITE_BLOCK_GREEN_NW = 1104,
  SPRITE_BLOCK_GREEN_N = 1105,
  SPRITE_BLOCK_GREEN_NE = 1106,
  SPRITE_BLOCK_GREEN_SW = 1108,
  SPRITE_BLOCK_GREEN_S = 1109,
  SPRITE_BLOCK_GREEN_SE = 1110,
  SPRITE_BLOCK_CYAN_NW = 1112,
  SPRITE_BLOCK_CYAN_N = 1113,
  SPRITE_BLOCK_CYAN_NE = 1114,
  SPRITE_BLOCK_CYAN_SW = 1116,
  SPRITE_BLOCK_CYAN_S = 1117,
  SPRITE_BLOCK_CYAN_SE = 1118,
  SPRITE_BLOCK_CYAN_W = 1120,
  SPRITE_BLOCK_CYAN_MID = 1121,
  SPRITE_BLOCK_CYAN_E = 1122,
  SPRITE_BLOCK_BROWN_NW = 1124,
  SPRITE_BLOCK_BROWN_N = 1125,
  SPRITE_BLOCK_BROWN_NE = 1126,
  SPRITE_BLOCK_BROWN_SW = 1128,
  SPRITE_BLOCK_BROWN_S = 1129,
  SPRITE_BLOCK_BROWN_SE = 1130,
  SPRITE_BLOCK_BROWN_W = 1132,
  SPRITE_BLOCK_BROWN_MID = 1133,
  SPRITE_BLOCK_BROWN_E = 1134,
  SPRITE_BLOCK_PINK_NW = 1144,
  SPRITE_BLOCK_PINK_N = 1145,
  SPRITE_BLOCK_PINK_NE = 1146,
  SPRITE_BLOCK_PINK_SW = 1148,
  SPRITE_BLOCK_PINK_S = 1149,
  SPRITE_BLOCK_PINK_SE = 1150,
  SPRITE_BLOCK_PINK_W = 1152,
  SPRITE_BLOCK_PINK_MID = 1153,
  SPRITE_BLOCK_PINK_E = 1154,
  SPRITE_BLOCK_BLUE_NW = 1156,
  SPRITE_BLOCK_BLUE_N = 1157,
  SPRITE_BLOCK_BLUE_NE = 1158,
  SPRITE_BLOCK_BLUE_SW = 1160,
  SPRITE_BLOCK_BLUE_S = 1161,
  SPRITE_BLOCK_BLUE_SE = 1162,
  SPRITE_BLOCK_BLUE_W = 1164,
  SPRITE_BLOCK_BLUE_MID = 1165,
  SPRITE_BLOCK_BLUE_E = 1166,
  SPRITE_CONES = 1196,
};
