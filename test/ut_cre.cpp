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
#include "../src/mvisenv.h"

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
private:
    void test_CreWnd();
private:
    Env* iEnv;
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_cre );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_cre, "Ut_cre");

static MDesSyncable* sSync;

void Ut_cre::setUp()
{
    sSync = NULL;
}

void Ut_cre::tearDown()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}

static bool Utcre_OnIdle(void* aData)
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
    return true;
}


static void OnIdle(void)
{
    if (sSync != NULL) {
	if (sSync->IsActive()) {
	    sSync->Update();
	}
	if (sSync->IsUpdated()) {
	    sSync->Confirm();
	}
    }
}

void Ut_cre::test_CreWnd()
{
    for (int ct = 1; ct < 2; ct++) {
	const string specn("ut_swnd_2");
	string ext = (ct == 0) ? "xml" : "chs";
	string spec = specn + string(".") + ext;
	string log = specn + "_" + ext + ".log";
	iEnv = new Env(spec, log);
	CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
	//iEnv->ImpsMgr()->ResetImportsPaths();
	iEnv->ImpsMgr()->AddImportsPaths("../modules");
	VisProv* visprov = new VisProv("VisProv", iEnv);
	iEnv->AddProvider(visprov);
	iEnv->ConstructSystem();
	MUnit* root = iEnv->Root();
	CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

	// Set idle handler
	MUnit* visenv = root->GetNode("./Test/Env/VisEnvAgt");
	CPPUNIT_ASSERT_MESSAGE("Fail to get env agent node", visenv != 0);
	MVisEnv* mvisenv = visenv->GetObj(mvisenv);
	CPPUNIT_ASSERT_MESSAGE("Fail to get env agent", mvisenv != 0);

	// Solid width output
	MUnit* sld_wo = root->GetNode("/testroot/Test/Model/Solid/WidthCp/Int");
	CPPUNIT_ASSERT_MESSAGE("Fail to get sld_wo ", sld_wo != NULL);

	// Sync the state
	MUnit* esync = root->GetNode("./Test/Capsule/Sync");
	CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
	MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
	CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
	sSync = sync;

	/*
	// Set GLES idle handler
	mvisenv->SetOnIdleHandler(OnIdle);
	// Start vis env loop
	mvisenv->Start();
	*/

	const TInt ticksnum = 128;
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
}



