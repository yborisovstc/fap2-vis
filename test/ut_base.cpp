#include <stdlib.h>
#include "../src/mwidget.h"
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <env.h>
#include <elem.h>
#include <mdes.h>
#include "../src/visprov.h"
#include <gdk/gdk.h>

#include <cppunit/extensions/HelperMacros.h>

/*
 * To run particular test suite: ./ut-fap2vis-lib [test_suite_name]
 * for instance ./ut-fap2vis-lib Ut_ExecMagt
 */

class Ut_base : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_base);
    CPPUNIT_TEST(test_Cre1);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    void test_Cre1();
private:
    Env* iEnv;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_base, "Ut_base");


void Ut_base::setUp()
{
}

void Ut_base::tearDown()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}

gboolean Utbase_OnIdle(gpointer aData)
{
}

void HandleGdkEvent(GdkEvent *event, gpointer data)
{
    GdkEventType type = event->type;
    GdkEventAny any = event->any;
    printf("Event %d, Wnd: %x\n", any.type, any.window);
    if (type == GDK_EXPOSE) {
	GdkEventExpose expose = event->expose;
	GdkWindow* window = expose.window;
	printf("Event EXPOSE\n");
	GdkGC* gc = gdk_gc_new(window);
	GdkColormap* cmap = gdk_rgb_get_colormap();
	gdk_gc_set_colormap(gc, cmap);
	GdkColor color;
	color.red = 65535;
	gdk_colormap_alloc_color(cmap, &color, 0, 1);
	gdk_gc_set_foreground(gc, &color);
	gdk_draw_rectangle(window, gc, TRUE, 5, 5, 5, 5);
    } else if (type == GDK_WINDOW_STATE) {
	GdkEventWindowState evt = event->window_state;
	printf("Event WINDOW_STATE, new state: %d\n", evt.new_window_state);
    }
}

void Ut_base::test_Cre1()
{
    printf("\n === Test of creating widget\n");

    // Init GDK
    g_type_init ();
    gdk_init(0, NULL);

    iEnv = new Env("ut_swnd_1.xml", "ut_swnd_1.txt");
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    //iEnv->ImpsMgr()->ResetImportsPaths();
    //iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    Elem* root = iEnv->Root();
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

    // Sync the state
    MElem* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);

    GdkDisplayManager* dm = gdk_display_manager_get();
    const gchar* dname = gdk_get_display_arg_name();
    GdkDisplay* d1 = gdk_display_open(NULL);
    gdk_display_manager_set_default_display (dm, d1);
    // GdkDisplay* dd = gdk_display_open_default_libgtk_only();

    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);
    GdkWindow* rootwnd = gdk_screen_get_root_window(screen);
    GdkWindowType rwtype = gdk_window_get_window_type(rootwnd);
    int rw_width = gdk_window_get_width(rootwnd);
    int rw_height = gdk_window_get_height(rootwnd);

    gdk_rgb_init();
    GdkWindowAttr attr;
    attr.window_type = GDK_WINDOW_TOPLEVEL;
    attr.x = 0;
    attr.y = 0;
    attr.width = 400;
    attr.height = 500;
    attr.colormap = gdk_rgb_get_cmap();
    attr.wclass = GDK_INPUT_OUTPUT;
    gint mask =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;


    GdkWindow* iWnd = gdk_window_new(rootwnd, &attr, mask); 
    GdkWindowType wtype = gdk_window_get_window_type(iWnd);
    GdkColor color;
    //gint cwres = gdk_color_white(attr.colormap, &color);
    color.green = 65535;
    gboolean cares = gdk_colormap_alloc_color(attr.colormap, &color, 0, 1);
    //gdk_window_move(iWnd, 200, 200);
    //gdk_window_move_resize(iWnd, 200, 200, 400, 300);
    gint cwres = gdk_color_white(attr.colormap, &color);
    //gdk_window_set_background(rootwnd, &color);
    gdk_window_show(rootwnd);
    gdk_window_set_background(iWnd, &color);
    gboolean wiv = gdk_window_is_visible(iWnd);
    gboolean rwiv = gdk_window_is_visible(rootwnd);
    GdkGC* wnd1gc = gdk_gc_new(iWnd);
    //gdk_gc_set_colormap(wnd1gc, attr.colormap);
    GdkColor color_drw1;
    color_drw1.red = 65535;
    cares = gdk_colormap_alloc_color(attr.colormap, &color_drw1, 0, 1);
    gdk_gc_set_foreground(wnd1gc, &color);
    gdk_draw_line(iWnd, wnd1gc, 5, 5, 40, 60); 
    gdk_draw_point(iWnd, wnd1gc, 2, 2);
    gdk_draw_rectangle(iWnd, wnd1gc, TRUE, 5, 5, 40, 60);
    gdk_window_show(iWnd);
    gdk_window_flush(iWnd);



    GdkWindowAttr attr2;
    attr2.window_type = GDK_WINDOW_CHILD;
    attr2.x = 20;
    attr2.y = 20;
    attr2.width = 40;
    attr2.height = 30;
    attr2.colormap = gdk_rgb_get_cmap();
    attr2.wclass = GDK_INPUT_OUTPUT;
    gint mask2 =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;
    GdkWindow* wnd2 = gdk_window_new(iWnd, &attr2, mask2); 
    gboolean wnd2en = gdk_window_ensure_native(wnd2);
    GdkWindow* wnd2prnt = gdk_window_get_parent(wnd2);
    gdk_window_move_resize(wnd2, 100, 120, 80, 50);
    //cwres = gdk_color_white(attr.colormap, &color);
    GdkColor color2;
    color2.green = 65535;
    cares = gdk_colormap_alloc_color(attr2.colormap, &color2, 0, 1);
    gdk_window_set_background(wnd2, &color2);
    //gdk_window_show(wnd2);
    //gdk_window_flush(wnd2);
    GdkWindowState wndst = gdk_window_get_state(iWnd);
    GdkWindowState wnd2st = gdk_window_get_state(wnd2);
    gdk_window_set_keep_above(wnd2, 1);
    gboolean wndhsn = gdk_window_has_native(iWnd);
    gboolean wnd2hsn = gdk_window_has_native(wnd2);
    gboolean wnd2comp = gdk_window_get_composited(wnd2);
    gboolean wnd2isv = gdk_window_is_viewable(wnd2);
    GdkWindowType wnd2type = gdk_window_get_window_type(wnd2);
    GdkEventMask wnd1_em = gdk_window_get_events(iWnd);
    printf("Wnd1 events mask: %x\n", wnd1_em);
    gboolean ep = gdk_events_pending();
    //GdkEvent* gev = gdk_event_peek();
    gdk_window_set_events(wnd2, GDK_ALL_EVENTS_MASK);
    GdkEventMask wnd2_em = gdk_window_get_events(wnd2);
    printf("Wnd2 events mask: %x\n", wnd2_em);

    GdkGC* wnd2gc = gdk_gc_new(wnd2);
    gdk_gc_set_colormap(wnd2gc, attr2.colormap);
    GdkColor color_drw2;
    color_drw2.red = 65535;
    cares = gdk_colormap_alloc_color(attr2.colormap, &color_drw2, 0, 1);
    gdk_gc_set_foreground(wnd2gc, &color_drw2);
    GdkRectangle wnd2ir = {5, 5, 20, 20};
    //gdk_gc_set_clip_rectangle(wnd2gc, &wnd2ir);
    gdk_window_invalidate_rect(wnd2, &wnd2ir, TRUE);
    gdk_draw_rectangle(wnd2, wnd2gc, 1, 5, 5, 20, 20);
    gdk_window_show(wnd2);
    //gdk_window_process_updates(wnd2, TRUE);
    gdk_window_flush(wnd2);

    // Wnd3 - offscreen
    /*
    GdkWindowAttr attr3;
    attr3.window_type = GDK_WINDOW_OFFSCREEN;
    attr3.x = 70;
    attr3.y = 70;
    attr3.width = 40;
    attr3.height = 30;
    attr3.colormap = gdk_rgb_get_cmap();
    attr3.wclass = GDK_INPUT_OUTPUT;
    gint mask3 =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;
    GdkWindow* wnd3 = gdk_window_new(iWnd, &attr3, mask3); 
    gdk_offscreen_window_set_embedder(wnd3, iWnd);
    GdkWindow* wnd3prnt = gdk_window_get_parent(wnd3);
    gdk_window_move_resize(wnd2, 50, 50, 40, 30);
    cwres = gdk_color_white(attr.colormap, &color);
    color.red = 65535;
    cares = gdk_colormap_alloc_color(attr.colormap, &color, 0, 1);
    gdk_window_set_background(wnd3, &color);
    gdk_window_show(wnd3);
    */
 


    gdk_event_handler_set(HandleGdkEvent, NULL, NULL);
    // Run Glib loop
    GMainLoop* mainloop = g_main_new (TRUE);
    //guint iares = g_idle_add(GSourceFunc function, gpointer data);
    g_main_run (mainloop);

    // Do some ticks
    const TInt ticksnum = 5;
    for (TInt cnt = 0; cnt < ticksnum; cnt++) {
	if (sync->IsActive()) {
	    sync->Update();
	}
	if (sync->IsUpdated()) {
	    sync->Confirm();
	}
    }

    delete iEnv;
}

