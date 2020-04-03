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
        $ + /AvrMdl/VrViewCp;
        $ + /ContainerMdl/VLayout;
        $ + /ContainerMdl/HLayout;
        $ + /ContainerMdl/Alignment;
    }
    Launcher : AVDesLauncher;
    Launcher < Path = /testroot/Test;
    Test : /testroot/Modules/DesComps/Des
    {
        $ # "Visualisation environment";
        Controller : /testroot/Modules/AvrMdl/VrController;
        Controller < {
            ModelPath < Value = "SS ./ut_vrc1_mdl.chs";
            DrpMp = /testroot/Test/Window/Scene/VBox/Slot_2/ModelView;
        }
        $ # "Visualisation environment";
        VisEnvAgt : AVisEnv;
        VisEnvAgt < Init = Yes;
        Window : /testroot/Modules/GVisComps/GWindow
        {
            VrpViewAgent : AVrpView;
            AWnd < Init = Yes;
            Width < Value = "SI 1200";
            Heigth < Value = "SI 800";
            VrvCp : /testroot/Modules/AvrMdl/VrViewCp;
            NodeSelected : AStatec;
            NodeSelected < Debug.Update = y;
            NodeSelected < Value = "SS nil";
            NodeSelectedReset : AStatec;
            NodeSelectedReset < Value = "SS nil";
            NodeSelectedReset ~ ./NodeSelected/Inp;
            NodeSelected ~ ./VrvCp/NavCtrl/NodeSelected;
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
            ./VrvCp/NavCtrl/CmdUp ~ ./Scene/VBox/Slot_1/Toolbar/Slot_1/BtnUp/Pressed;
        }
        EnvWidth : AStatec;
        EnvHeight : AStatec;
        Title : AStatec;
        EnvWidth ~ ./Window/Inp_W;
        EnvHeight ~ ./Window/Inp_H;
        Title ~ ./Window/Inp_Title;
        ./Controller/CtrlCp ~ ./Window/VrvCp;
    }
}
