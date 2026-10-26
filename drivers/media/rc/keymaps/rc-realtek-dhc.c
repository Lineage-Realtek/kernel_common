// SPDX-License-Identifier: GPL-2.0+
// Keytable for realtek_dhc Remote Controller
//
// keymap imported from ir-keymaps.c
//
// Copyright (c) 2020 by Simon Hsu

#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table realtek_dhc[] = {
	// address 0xDF
	{ 0xDF1C, KEY_POWER },
	{ 0xDF4B, KEY_VIDEO_PREV },
	{ 0xDF4F, KEY_VIDEO_NEXT },
	{ 0xDF01, KEY_PLAY },
	{ 0xDF5F, KEY_F14 },
	{ 0xDF5D, KEY_VOLUMEUP },
	{ 0xDF5C, KEY_VOLUMEDOWN },
	{ 0xDF42, KEY_HOME },
	{ 0xDF0A, KEY_BACK },
	{ 0xDF47, KEY_LEFT },
	{ 0xDF1A, KEY_UP },
	{ 0xDF07, KEY_RIGHT },
	{ 0xDF48, KEY_DOWN },
	{ 0xDF06, KEY_ENTER },
	{ 0xDF03, BTN_MOUSE },
	{ 0xDF18, KEY_MENU },
	{ 0xDF54, KEY_1 },
	{ 0xDF16, KEY_2 },
	{ 0xDF15, KEY_3 },
	{ 0xDF50, KEY_4 },
	{ 0xDF12, KEY_5 },
	{ 0xDF11, KEY_6 },
	{ 0xDF4C, KEY_7 },
	{ 0xDF0E, KEY_8 },
	{ 0xDF0D, KEY_9 },
	{ 0xDF0C, KEY_0 },
	{ 0xDF41, KEY_MUTE },
	{ 0xDF10, KEY_BACKSPACE },

	// address 0x1FE
	{ 0x1FE40, KEY_POWER },
	{ 0x1FE44, KEY_BLUE },
	{ 0x1FE55, KEY_REWIND },
	{ 0x1FE5A, KEY_PLAY },
	{ 0x1FE52, KEY_STOP },
	{ 0x1FE54, KEY_FASTFORWARD },
	{ 0x1FE43, KEY_F14 },
	{ 0x1FE0F, KEY_F15 },
	{ 0x1FE10, KEY_VOLUMEDOWN },
	{ 0x1FE18, KEY_VOLUMEUP },
	{ 0x1FE11, KEY_HOME },
	{ 0x1FE19, KEY_BACK },
	{ 0x1FE51, KEY_LEFT },
	{ 0x1FE16, KEY_UP },
	{ 0x1FE50, KEY_RIGHT },
	{ 0x1FE1A, KEY_DOWN },
	{ 0x1FE13, KEY_ENTER },
	{ 0x1FE4C, KEY_MENU },
	{ 0x1FE00, BTN_MOUSE },
	{ 0x1FE4E, KEY_1 },
	{ 0x1FE0D, KEY_2 },
	{ 0x1FE0C, KEY_3 },
	{ 0x1FE4A, KEY_4 },
	{ 0x1FE09, KEY_5 },
	{ 0x1FE08, KEY_6 },
	{ 0x1FE46, KEY_7 },
	{ 0x1FE05, KEY_8 },
	{ 0x1FE04, KEY_9 },
	{ 0x1FE01, KEY_0 },
	{ 0x1FE41, KEY_MUTE },
	{ 0x1FE42, KEY_BACKSPACE },
	{ 0x1FEF5, KEY_F21 },
	{ 0x1FE5E, KEY_PAGEUP },
	{ 0x1FE5C, KEY_PAGEDOWN },
	{ 0x1FEF1, KEY_RED },
	{ 0x1FEF2, KEY_GREEN },
	{ 0x1FEF3, KEY_YELLOW },
	{ 0x1FEF4, KEY_YELLOW },

	// address 0x20DF
	{ 0x20DF5A, KEY_F18 },
	{ 0x20DF56, KEY_F19 },
	{ 0x20DF51, KEY_F20 },
	{ 0x20DF0D, KEY_F12 },

	// address 0x807F
	{ 0x807F18, KEY_POWER },
	{ 0x807F32, KEY_TV },
	{ 0x807F05, KEY_RED },
	{ 0x807F09, KEY_GREEN },
	{ 0x807F42, KEY_YELLOW },
	{ 0x807F43, KEY_BLUE },
	{ 0x807F4F, KEY_REWIND },
	{ 0x807F55, KEY_PLAY },
	{ 0x807F16, KEY_FASTFORWARD },
	{ 0x807F4E, KEY_PROG1 },
	{ 0x807F4A, KEY_PROG2 },
	{ 0x807F4D, KEY_UP },
	{ 0x807F48, KEY_DOWN },
	{ 0x807F0E, KEY_RIGHT },
	{ 0x807F0C, KEY_LEFT },
	{ 0x807F4C, KEY_ENTER },
	{ 0x807F57, KEY_BACK },
	{ 0x807F14, KEY_HOME },
	{ 0x807F66, KEY_APPSELECT },
	{ 0x807F4B, KEY_VOLUMEDOWN },
	{ 0x807F49, KEY_VOLUMEUP },
	{ 0x807F74, KEY_ASSISTANT },
	{ 0x807F0D, KEY_MUTE },
	{ 0x807F59, KEY_CHANNELUP },
	{ 0x807F5F, KEY_CHANNELDOWN },
	{ 0x807F0B, BTN_MOUSE },
	{ 0x807F21, KEY_1 },
	{ 0x807F22, KEY_2 },
	{ 0x807F23, KEY_3 },
	{ 0x807F24, KEY_4 },
	{ 0x807F25, KEY_5 },
	{ 0x807F26, KEY_6 },
	{ 0x807F27, KEY_7 },
	{ 0x807F28, KEY_8 },
	{ 0x807F29, KEY_9 },
	{ 0x807F20, KEY_0 },
	{ 0x807F5A, KEY_SELECT },
	{ 0x807F30, KEY_LAST },
	{ 0x807F58, KEY_CYCLEWINDOWS },
	{ 0x807F56, KEY_OPTION },
	{ 0x807F54, KEY_INFO },
	{ 0x807F17, KEY_STOP },
	{ 0x807F08, KEY_PREVIOUS },
	{ 0x807F77, KEY_E },

	// address 0x8877
	{ 0x887721, KEY_POWER },
	{ 0x887760, KEY_SELECT },
	{ 0x887701, KEY_1 },
	{ 0x887702, KEY_2 },
	{ 0x887703, KEY_3 },
	{ 0x887704, KEY_4 },
	{ 0x887705, KEY_5 },
	{ 0x887706, KEY_6 },
	{ 0x887707, KEY_7 },
	{ 0x887708, KEY_8 },
	{ 0x887709, KEY_9 },
	{ 0x88770A, KEY_0 },
	{ 0x887758, KEY_SUBTITLE },
	{ 0x887729, KEY_INFO },
	{ 0x88774B, KEY_RED },
	{ 0x88774A, KEY_GREEN },
	{ 0x887749, KEY_YELLOW },
	{ 0x88774C, KEY_BLUE },
	{ 0x887774, KEY_BOOKMARKS },
	{ 0x887759, KEY_PROG3 },
	{ 0x887746, KEY_ASSISTANT },
	{ 0x88770F, KEY_SETUP },
	{ 0x887710, KEY_SETUP },
	{ 0x887715, KEY_UP },
	{ 0x887716, KEY_DOWN },
	{ 0x887717, KEY_LEFT },
	{ 0x887718, KEY_RIGHT },
	{ 0x887719, KEY_ENTER },
	{ 0x887748, KEY_BACK },
	{ 0x887747, KEY_HOME },
	{ 0x887732, KEY_PROG4 },
	{ 0x887723, KEY_VOLUMEUP },
	{ 0x887724, KEY_VOLUMEDOWN },
	{ 0x887725, KEY_MUTE },
	{ 0x887733, KEY_CHANNELUP },
	{ 0x887734, KEY_CHANNELDOWN },
	{ 0x887764, KEY_PROG2 },
	{ 0x887763, KEY_PROG1 },
	{ 0x887767, KEY_Q },
	{ 0x887768, KEY_W },
	{ 0x887773, KEY_R },
	{ 0x88771F, KEY_T },
	{ 0x887769, KEY_Y },
	{ 0x887770, KEY_U },
};

static struct rc_map_list realtek_dhc_map = {
	.map = {
		.scan     = realtek_dhc,
		.size     = ARRAY_SIZE(realtek_dhc),
		.rc_proto = RC_PROTO_NECX,
		.name     = RC_MAP_REALTEK_DHC,
	}
};

static int __init init_rc_map_realtek_dhc(void)
{
	return rc_map_register(&realtek_dhc_map);
}

static void __exit exit_rc_map_realtek_dhc(void)
{
	rc_map_unregister(&realtek_dhc_map);
}

module_init(init_rc_map_realtek_dhc)
module_exit(exit_rc_map_realtek_dhc)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simon Hsu <simon_hsu@realtek.com>");
