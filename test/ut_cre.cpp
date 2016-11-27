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

class Ut_cre : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_cre);
    CPPUNIT_TEST(test_CreWnd);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
    void test_CreWnd();
private:
    Env* iEnv;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_cre, "Ut_cre");


void Ut_cre::setUp()
{
}

void Ut_cre::tearDown()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}

gboolean Utcre_OnIdle(gpointer aData)
{
    MDesSyncable* sync = (MDesSyncable*) aData;
    // Tick
    //printf("Tick\n");
    if (sync->IsActive()) {
	sync->Update();
    }
    if (sync->IsUpdated()) {
	sync->Confirm();
    }
    return TRUE;
}

void Ut_cre::test_CreWnd()
{
    iEnv = new Env("ut_swnd_1.xml", "ut_swnd_1.txt");
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    //iEnv->ImpsMgr()->ResetImportsPaths();
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
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

    // Run Glib loop
    GMainLoop* mainloop = g_main_new (TRUE);
    guint iares = g_idle_add(Utcre_OnIdle, sync);
    g_main_run (mainloop);

    delete iEnv;
}



