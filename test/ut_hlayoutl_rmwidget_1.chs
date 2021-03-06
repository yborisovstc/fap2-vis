testroot : Elem
{
    $ # "Unit test of Container base agent";
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
        $ + /FvWidgetsL/FButtonL;
        $ + /ContainerModL/FVLayoutL;
        $ + /ContainerModL/FVLayoutLSlot;
        $ + /ContainerModL/FHLayoutL;
        $ + /ContainerModL/FHLayoutLSlot;
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
            AWnd < Init = Yes;
            Width < Value = "SI 1200";
            Heigth < Value = "SI 800";
            Scene : /testroot/Modules/GVisComps/Scene
            {
                HBox : /*/Modules/ContainerModL/FHLayoutL
                {
                    Padding = 20;
                    AlcW < Value = "SI 220";
                    AlcH < Value = "SI 330";
                    Btn1 : /*/Modules/FvWidgetsL/FButtonL
                    {
                        Text = "Button 1";
                        BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                    Slot_1 : /*/Modules/ContainerModL/FHLayoutLSlot;
                    ./Slot_1/SCp ~ ./Btn1/Cp;
                    Btn2 : /*/Modules/FvWidgetsL/FButtonL
                    {
                        Text = "Button 2";
                        BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                    Slot_2 : /*/Modules/ContainerModL/FHLayoutLSlot;
                    ./Slot_2/SCp ~ ./Btn2/Cp;
                    ./Slot_2/Next ~ ./Slot_1/Prev;
                    ./Slot_1/Next ~ Start;
                    Slot_2/Prev ~ End;
                }
            }
        }
        EnvWidth : AStatec;
        EnvHeight : AStatec;
        Title : AStatec;
        EnvWidth ~ ./Window/Inp_W;
        EnvHeight ~ ./Window/Inp_H;
        Title ~ ./Window/Inp_Title;
        $ # "Mutation to remove Bnt2";
        Timeout : AStatec {
            Debug.Update = y;
            Debug.LogLevel = 0;
            Value = "SI 0";
        }
        Timeout/Inp ~ : ATrcAddVar @ {
            Inp ~ Timeout;
            Inp ~ : AStatec {
                Value = "SI 1";
            };
        };
        Window/Scene/HBox/InpMutRmWidget ~ : ATrcSwitchBool @ {
            Sel ~ Cmp_Eq : ATrcCmpVar @ {
                Inp ~ Timeout;
                Inp2 ~ : AStatec {
                    Value = "SI 20";
                };
            };
            Cmp_Eq < Debug.LogLevel = 20;
            Inp1 ~ : AStatec {
                Value = "SI -1";
            };
            Inp2 ~ : AStatec {
                Value = "SI 1";
            };
        };
    }
}
