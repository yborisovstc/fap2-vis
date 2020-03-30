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
        $ + /FvWidgets/FButton;
        $ + /AvrMdl/UnitDrp;
        $ + /AvrMdl/VrController;
        $ + /ContainerMdl/VLayout;
        $ + /ContainerMdl/HLayout;
        $ + /ContainerMdl/Alignment;
    }
    Comps : Elem;
    Test : /testroot/Modules/DesComps/Des
    {
        $ # "Visualisation environment";
        Controller : /testroot/Modules/AvrMdl/VrController;
        Controller < {
            ModelPath < Value = "SS ./ut_vrc1_mdl.chs";
            DrpMp = /testroot/Test/Env/Window/Scene/VBox/Slot_2/ModelView;
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
                    VBox : /*/Modules/ContainerMdl/VLayout
                    {
                        About = "Application view main vertical layout";
                        Slot_1 : AVSlot;
                        Slot_1 < Toolbar : /*/Modules/ContainerMdl/HLayout
                        {
                            About = "Application toolbar";
                            Padding = 2;
                            Slot_1 : AVSlot;
                            ./Slot_1 < BtnUp : /*/Modules/FvWidgets/FButton
                            {
                                Text = Up;
                                BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                                FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            }
                            Slot_2 : AVSlot;
                            ./Slot_2 < Btn2 : /*/Modules/FvWidgets/FButton
                            {
                                Text = "Button 2";
                                BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                                FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            }
                            Slot_3 : AVSlot;
                            ./Slot_3 < Btn2 : /*/Modules/FvWidgets/FButton
                            {
                                Text = "Button 3";
                                BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                                FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            }
                        }
                        Slot_2 : AVSlot;
                        Slot_2 < ModelView : /*/Modules/ContainerMdl/Alignment;
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
