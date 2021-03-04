testroot : Elem
{
    $ # "Models navigator";
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
        $ + /AvrMdl/SystDrp;
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
        VisEnvAgt : AVisEnv;
        VisEnvAgt < Init = Yes;
        Window : /testroot/Modules/GVisComps/GWindow
        {
            VrpViewAgent : AVrpView;
            AWnd < Init = Yes;
            Width < Value = "SI 1200";
            Height < Value = "SI 800";
            VrvCp : /testroot/Modules/AvrMdl/VrViewCp;
            ./VrvCp/NavCtrl/NodeSelected ~ NodeSelected : AStatec
            {
                Debug.Update = y;
                Value = "SS nil";
            };
            ./NodeSelected/Inp ~ NodeSelectedReset : AStatec {
                Value = "SS nil";
            };
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
                    TestMvCompsCount : AStatec {
                        Debug.Update = y;
                        Value = SI;
                    }
                    TestMvCompsCount/Inp ~ ModelView/OutCompsCount;
                }
                Scp : /*/Modules/ContainerModL/SlotCp;
                Scp ~ ./VBox/Cp;
                ./Scp/InpAlcW ~ ./Cp/Width;
                ./Scp/InpAlcH ~ ./Cp/Height;
            }
            ./Scene/Cp ~ ScCpc;
            ./VrvCp/NavCtrl/CmdUp ~ ./Scene/VBox/Toolbar/BtnUp/Pressed;
            ./Scene/VBox/ModelView/InpMutAddWidget ~ ./VrvCp/NavCtrl/MutAddWidget;
            ./Scene/VBox/ModelView/OutCompsCount ~ ./VrvCp/NavCtrl/DrpCreated;
            ./Scene/VBox/ModelView/OutCompsCount ~ ./VrvCp/NavCtrl/VrvCompsCount;
        }
        EnvWidth : AStatec;
        EnvHeight : AStatec;
        Title : AStatec;
        EnvWidth ~ ./Window/Inp_W;
        EnvHeight ~ ./Window/Inp_H;
        Title ~ ./Window/Inp_Title;
        $ # "Visual representation controller";
        Controller : /testroot/Modules/AvrMdl/VrController;
        Controller < {
            DrpMp = /testroot/Test/Window/Scene/VBox/ModelView;
            ModelViewUdp/AdpAgent < AgentUri = /testroot/Test/Window/Scene/VBox/ModelView;
            WindowEdp/AdpAgent < AgentUri = /testroot/Test/Window;
            $ # " Just interim solution";
            Const_SMdlRoot < Value = "SS /*/Test/Controller/ModelMnt/*";
        }
        ./Controller/CtrlCp ~ ./Window/VrvCp;
    }
}
