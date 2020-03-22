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

#include <GLFW/glfw3.h>

/*
 * To run particular test suite: ./ut-fap2vis-lib [test_suite_name] for instance ./ut-fap2vis-lib Ut_ExecMagt
 */


class AgentObserver: public MAgentObserver
{
    public:
	AgentObserver() {}
	// From MAgentObserver
	virtual void OnCompDeleting(MUnit& aComp, TBool aSoft = ETrue, TBool aModif = EFalse) {}
	virtual void OnCompAdding(MUnit& aComp, TBool aModif = EFalse) {}
	virtual TBool OnCompChanged(MUnit& aComp, const string& aContName = string(), TBool aModif = EFalse) {}
	virtual TBool OnChanged(MUnit& aComp) {}
	virtual TBool OnCompRenamed(MUnit& aComp, const string& aOldName) {}
	virtual void OnCompMutated(const MUnit* aNode) {}
	virtual void OnError(const MUnit* aComp) { mClose = true;}
	MIface* Call(const string& aSpec, string& aRes) override { return NULL;};
	string Mid() const override { return string();}
    public:
	bool mClose = false;
};


/** @brief Widgets UT
 * */
class Ut_cnt : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_cnt);
    //CPPUNIT_TEST(test_Base);
    CPPUNIT_TEST(testVlayout1);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
private:
    void test_Base();
    void testVlayout1();
private:
    Env* iEnv;
    AgentObserver mAgtObs;
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_cnt );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_cnt, "Ut_cnt");

static MDesSyncable* sSync;

void Ut_cnt::setUp()
{
    sSync = NULL;
}

void Ut_cnt::tearDown()
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

void Ut_cnt::test_Base()
{
    // Test of simple containter - horisontal layout
    const string specn("ut_cnt1");
    string ext = "chs";
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

    // Wdg1 requisition, width output
    MUnit* w1_rqwo = root->GetNode("/testroot/Test/Env/Window/Scene/Cnt1/Slot_1/Wdg1/OutRqsW/Int");
    CPPUNIT_ASSERT_MESSAGE("Fail to get w1_rqwo", w1_rqwo != NULL);

    // Container #1
    MUnit* cnt1 = root->GetNode("/testroot/Test/Env/Window/Scene/Cnt1");
    CPPUNIT_ASSERT_MESSAGE("Fail to get cnt1", cnt1 != NULL);

    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
    sSync = sync;

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

void Ut_cnt::testVlayout1()
{
    printf("\n === Vertical layout test 1\n");
    const string specn("ut_vlayout_1");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    //iEnv->ImpsMgr()->ResetImportsPaths();
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->SetObserver(&mAgtObs);
    iEnv->ConstructSystem();
    MUnit* root = iEnv->Root();
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

    // Set idle handler
    MUnit* visenv = root->GetNode("./Test/Env/VisEnvAgt");
    CPPUNIT_ASSERT_MESSAGE("Fail to get env agent node", visenv != 0);
    MVisEnv* mvisenv = visenv->GetObj(mvisenv);
    CPPUNIT_ASSERT_MESSAGE("Fail to get env agent", mvisenv != 0);
    
    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
    sSync = sync;

    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

#if 0
    const TInt ticksnum = 128;
    //for (TInt cnt = 0; cnt < ticksnum; cnt++) {
    while (!mAgtObs.mClose) {
	if (sync->IsActive()) {
	    sync->Update();
	} else if (sync->IsUpdated()) {
	    sync->Confirm();
	} else {
	    // Handle idle
	    glfwPollEvents();
	}
    }
#endif
    delete iEnv;
}

