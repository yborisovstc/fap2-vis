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
#include "../src/mwindow.h"
#include "../src/mscel.h"

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
	virtual void OnCompDeleting(const MUnit* aComp, TBool aSoft = ETrue, TBool aModif = EFalse) override {}
	virtual void OnCompAdding(const MUnit* aComp, TBool aModif = EFalse) override {}
	virtual TBool OnCompChanged(const MUnit* aComp, const string& aContName = string(), TBool aModif = EFalse) override {}
	virtual TBool OnChanged(const MUnit* aComp) override {}
	virtual TBool OnCompRenamed(const MUnit* aComp, const string& aOldName) override {}
	virtual void OnCompMutated(const MUnit* aNode) override {}
	virtual void OnError(const MUnit* aComp) override { mClose = true;}
	MIface* Call(const string& aSpec, string& aRes) override { return NULL;};
	string Mid() const override { return string();}
    public:
	bool mClose = false;
};


/** @brief Agents visual representatin UT
 * */
class Ut_avr : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_avr);
//    CPPUNIT_TEST(test_Unit);
//    CPPUNIT_TEST(test_UnitDrp);
    CPPUNIT_TEST(test_VrCtrl);
//    CPPUNIT_TEST(test_SystDrp);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
private:
    void test_Unit();
    void test_UnitDrp();
    void test_VrCtrl();
    void test_SystDrp();
private:
    Env* iEnv;
    AgentObserver mAgtObs;
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_avr );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_avr, "Ut_avr");

static MDesSyncable* sSync;

void Ut_avr::setUp()
{
    sSync = NULL;
}

void Ut_avr::tearDown()
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

void Ut_avr::test_Unit()
{
    printf("\n === Unit CRP test\n");
    const string specn("ut_avr_unit");
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

    const TInt ticksnum = 128;
    for (TInt cnt = 0; cnt < ticksnum; cnt++) {
    //while (!mAgtObs.mClose) {
	if (sync->IsActive()) {
	    sync->Update();
	} else if (sync->IsUpdated()) {
	    sync->Confirm();
	} else {
	    // Handle idle
	    glfwPollEvents();
	}
    }
    delete iEnv;
}

void Ut_avr::test_UnitDrp()
{
    printf("\n === Unit DRP test 1\n");
    const string specn("ut_avr_unit_drp");
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

    // Verify getting window from widget
    MUnit* wdga = root->GetNode("./Test/Window/Scene/Drp/Slot_1/Wdg1/WdgAgent");
    CPPUNIT_ASSERT_MESSAGE("Fail to get WdgAgent", wdga != 0);
    MWindow* mwnd = (MWindow*) wdga->GetSIfi(MWindow::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", mwnd != 0);
    
    // Verify resolving scene elems in container
    MUnit* cntu = root->GetNode("./Test/Env/Window/Scene/Drp/CntAgent");
    CPPUNIT_ASSERT_MESSAGE("Fail to get Drp", cntu != 0);
    cntu->GetIfi(MSceneElem::Type());

    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
    sSync = sync;

    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}

void Ut_avr::test_VrCtrl()
{
    printf("\n === VR Controller test 1\n");
    const string specn("ut_avr_vrc_1l");
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

    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
    sSync = sync;

    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}

void Ut_avr::test_SystDrp()
{
    printf("\n === System DRP test 0\n");
    const string specn("ut_avr_vrc_1l");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    //iEnv->ImpsMgr()->ResetImportsPaths();
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->SetEVar("Model","./test_model_syst_1.chs");
    iEnv->ConstructSystem();
    MUnit* root = iEnv->Root();
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

    // Sync the state
    MUnit* esync = root->GetNode("./Test/Capsule/Sync");
    CPPUNIT_ASSERT_MESSAGE("Fail to get input for Syncable iface", esync != 0);
    MDesSyncable* sync = (MDesSyncable*) esync->GetSIfi(MDesSyncable::Type());
    CPPUNIT_ASSERT_MESSAGE("Fail to get Syncable iface", sync != 0);
    sSync = sync;

    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}
