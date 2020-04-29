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



/** @brief Test of container using approach of widget assosiation to slots via link
 * */
class Ut_cntl : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_cntl);
    //CPPUNIT_TEST(testVlayout1);
    //CPPUNIT_TEST(testVlayoutCmb);
    CPPUNIT_TEST(testVlayoutCmb2);
    //CPPUNIT_TEST(testHlayout1);
    //CPPUNIT_TEST(testHlayout2);
    CPPUNIT_TEST_SUITE_END();
    public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testVlayout1();
    void testVlayoutCmb();
    void testVlayoutCmb2();
    void testHlayout1();
    void testHlayout2();
private:
    Env* iEnv;
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_cntl );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Ut_cntl, "Ut_cntl");


void Ut_cntl::setUp()
{
}

void Ut_cntl::tearDown()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


void Ut_cntl::testVlayout1()
{
    printf("\n === Vertical layout (SLW approach) test 1\n");
    const string specn("ut_vlayoutl_1");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    MUnit* root = iEnv->Root();
    CPPUNIT_ASSERT_MESSAGE("Fail to get root", root != 0);

    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}


void Ut_cntl::testVlayoutCmb()
{
    printf("\n === Combined Vertical layout (SLW approach) test 1\n");
    const string specn("ut_vlayoutl_2");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}

void Ut_cntl::testVlayoutCmb2()
{
    printf("\n === Combined Vertical layout (SLW approach) test 1\n");
    const string specn("ut_vlayoutl_3");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}

void Ut_cntl::testHlayout1()
{
    printf("\n === Single horisontal layout (SLW approach) test 1\n");
    const string specn("ut_hlayoutl_1");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}

void Ut_cntl::testHlayout2()
{
    printf("\n === Combined horisontal layout (SLW approach) test 1\n");
    const string specn("ut_hlayoutl_2");
    string ext = "chs";
    string spec = specn + string(".") + ext;
    string log = specn + "_" + ext + ".log";
    iEnv = new Env(spec, log);
    CPPUNIT_ASSERT_MESSAGE("Fail to create Env", iEnv != 0);
    iEnv->ImpsMgr()->AddImportsPaths("../modules");
    VisProv* visprov = new VisProv("VisProv", iEnv);
    iEnv->AddProvider(visprov);
    iEnv->ConstructSystem();
    bool run = iEnv->RunSystem();
    CPPUNIT_ASSERT_MESSAGE("Fail to run system", run);

    delete iEnv;
}
