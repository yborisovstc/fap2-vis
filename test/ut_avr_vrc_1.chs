testroot : Elem
{
    $ # "UT of Unit DRP";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /IncapsComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcOutExt;
        $ + /DesComps/StcInpExt;
        $ + /GVisComps/GWindow;
        $ + /GVisComps/VisEnv;
        $ + /GVisComps/Scene;
        $ + /GVisComps/SceRect;
        $ + /FvWidgets/FUnitCrp;
        $ + /AvrMdl/UnitDrp;
        $ + /AvrMdl/VrController;
    }
    Comps : Elem;
    Test : /testroot/Modules/DesComps/Des
    {
        $ # "Visualisation environment";
        Controller : /testroot/Modules/AvrMdl/VrController;
        Controller < {
            ModelPath < Value = "SS ./ut_vrc1_mdl.chs";
            Scene = /testroot/Test/Env/Window/Scene;
        }
        Env : /testroot/Modules/GVisComps/VisEnv
        {
            $ # "Visualisation environment";
            VisEnvAgt < Init = Yes;
            Window : /testroot/Modules/GVisComps/GWindow
            {
                AWnd < Init = Yes;
                Width < Value = "SI 1200";
                Heigth < Value = "SI 800";
                Scene : /testroot/Modules/GVisComps/Scene
                {
                    $ # "Visualisation scene";
                    Drp : /*/Modules/AvrMdl/UnitDrp
                    {
                        Padding = 10;
                    }
                }
            }
            EnvWidth : AStatec;
            EnvHeight : AStatec;
            Title : AStatec;
            EnvWidth ~ ./Window/Inp_W;
            EnvHeight ~ ./Window/Inp_H;
            Title ~ ./Window/Inp_Title;
        }
        ./Env/EnvWidth < Value = "SI 640";
        ./Env/EnvHeight < Value = "SI 480";
        ./Env/Title < Value = "SS Title";
    }
}
