#include <stdlib.h>
//#include "../src/mwidget.h"
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <env.h>
#include <elem.h>
#include <mdes.h>
#include "../src/visprov.h"

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
    printf("Event %d, Wnd: %p\n", any.type, any.window);
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
    printf("\n === Test of creating window\n");

    // Init GDK
    g_type_init ();
    gdk_init(0, NULL);

    iEnv = new Env("ut_swnd_1.xml", "ut_swnd_1.txt");
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    //iEnv->ImpsMgr()->ResetImportsPaths();
    //iEnv->ImpsMgr()->AddImportsPaths("../modules");
    // Don't add provider if provider is installed as plugin
    /*
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    */
    iEnv->ConstructSystem();
    MUnit* root = iEnv->Root();
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);

    delete iEnv;
}

