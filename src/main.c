#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/screensaver.h>

xcb_connection_t *conn = NULL;

xcb_window_t get_root_window()
{
	const xcb_setup_t* setup;
	xcb_screen_t* screen;
	setup = xcb_get_setup(conn);
	screen = xcb_setup_roots_iterator(setup).data;
	return screen->root;
}

xcb_screensaver_query_info_reply_t* get_screensaver_info(xcb_window_t rootwin)
{
	xcb_screensaver_query_info_cookie_t cookie;
	xcb_screensaver_query_info_reply_t* reply;

	cookie = xcb_screensaver_query_info(conn, rootwin);
	reply = xcb_screensaver_query_info_reply(conn, cookie, NULL);

	return reply;
}

#define PRINT_KEY_VALUE(KEY, FMT, VALUE) printf("%s\t" FMT "\n", KEY, VALUE)
void print_screensaver_info(xcb_screensaver_query_info_reply_t* info)
{
	PRINT_KEY_VALUE("state", "%u", info->state);
	PRINT_KEY_VALUE("kind", "%u", info->kind);
	PRINT_KEY_VALUE("til", "%u", info->ms_until_server);
	PRINT_KEY_VALUE("idle", "%u", info->ms_since_user_input);
	PRINT_KEY_VALUE("event_mask", "%u", info->event_mask);
}
#undef PRINT_KEY_VALUE

int main(int argc, char ** argv)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
	{
		fprintf(stderr, "Error opening display.\n");
		return 1;
	}

	xcb_window_t rootwin = get_root_window();

	xcb_screensaver_query_info_reply_t* info = get_screensaver_info(rootwin);

	if (info == NULL)
	{
		fprintf(stderr, "Failed to get info.\n");
		return 2;
	}

	print_screensaver_info(info);
	free(info);

	return 0;
}
