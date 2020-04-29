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
        $ + /GVisComps/SceneCp;
        $ + /GVisComps/Scene;
        $ + /GVisComps/SceRect;
        $ + /FvWidgetsL/FUnitCrp;
        $ + /AvrMdl/UnitDrp;
        $ + /AvrMdl/VrController;
        $ + /AvrMdl/VrViewCp;
        $ + /FvWidgetsL/FButtonL;
        $ + /ContainerModL/FVLayoutL;
        $ + /ContainerModL/FVLayoutLSlot;
        $ + /ContainerModL/FHLayoutL;
        $ + /ContainerModL/FHLayoutLSlot;
        $ + /ContainerModL/AlignmentSlot;
        $ + /ContainerModL/Alignment;
        $ + /ContainerModL/SlotCp;
    }
    Launcher : AVDesLauncher;
    Launcher < Path = /testroot/Test;
    Test : /testroot/Modules/DesComps/Des
    {
        $ # "Visualisation environment";
        Controller : /testroot/Modules/AvrMdl/VrController;
        Controller < {
            ModelPath < Value = "SS ./ut_vrc1_mdl.chs";
            DrpMp = /testroot/Test/Window/Scene/VBox/ModelView;
        }
        $ # "Visualisation environment";
        VisEnvAgt : AVisEnv;
        VisEnvAgt < Init = Yes;
        Window : /testroot/Modules/GVisComps/GWindow
        {
            VrpViewAgent : AVrpView;
            AWnd < Init = Yes;
            Width < Value = "SI 1200";
            Height < Value = "SI 800";
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
                VBox : /*/Modules/ContainerModL/FVLayoutL
                {
                    About = "Application view main vertical layout";
                    Slot_1 : /*/Modules/ContainerModL/FVLayoutLSlot;
                    ./Slot_1/Next ~ ./Start;
                    Toolbar : /*/Modules/ContainerModL/FHLayoutL
                    {
                        About = "Application toolbar";
                        ./Padding < Value = "SI 2";
                        Slot_1 : /*/Modules/ContainerModL/FHLayoutLSlot;
                        ./Slot_1/Next ~ ./Start;
                        BtnUp : /*/Modules/FvWidgetsL/FButtonL
                        {
                            Text = Up;
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        ./Slot_1/SCp ~ ./BtnUp/Cp;
                        Slot_2 : /*/Modules/ContainerModL/FHLayoutLSlot;
                        ./Slot_2/Next ~ ./Slot_1/Prev;
                        Btn2 : /*/Modules/FvWidgetsL/FButtonL
                        {
                            Text = "Button 2";
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        ./Slot_2/SCp ~ ./Btn2/Cp;
                        Slot_3 : /*/Modules/ContainerModL/FHLayoutLSlot;
                        ./Slot_3/Next ~ ./Slot_2/Prev;
                        ./End ~ ./Slot_3/Prev;
                        Btn3 : /*/Modules/FvWidgetsL/FButtonL
                        {
                            Text = "Button 3";
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        ./Slot_3/SCp ~ ./Btn3/Cp;
                    }
                    ./Slot_1/SCp ~ ./Toolbar/Cp;
                    Slot_2 : /*/Modules/ContainerModL/FVLayoutLSlot;
                    ModelView : /*/Modules/ContainerModL/Alignment;
                    ./Slot_2/SCp ~ ./ModelView/Cp;
                    ./Slot_2/Next ~ ./Slot_1/Prev;
                    ./End ~ ./Slot_2/Prev;
                }
                Scp : /*/Modules/ContainerModL/SlotCp;
                Scp ~ ./VBox/Cp;
                ./Scp/InpAlcW ~ ./Cp/Width;
                ./Scp/InpAlcH ~ ./Cp/Height;
            }
            ./Scene/Cp ~ ScCpc;
            ./VrvCp/NavCtrl/CmdUp ~ ./Scene/VBox/Toolbar/BtnUp/Pressed;
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
