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
                    ./Padding < Value = "SI 40";
                    AlcW < Value = "SI 220";
                    AlcH < Value = "SI 330";
                    $ # " ==== Slot 1 ====";
                    VBox2 : /*/Modules/ContainerModL/FVLayoutL
                    {
                        ./Padding < Value = "SI 10";
                        ./AlcW < Value = "SI 220";
                        ./AlcH < Value = "SI 330";
                        Btn2_1 : /*/Modules/FvWidgetsL/FButtonL
                        {
                            Text = "Button 2_1";
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        Slot_1 : /*/Modules/ContainerModL/FVLayoutLSlot;
                        ./Slot_1/SCp ~ ./Btn2_1/Cp;
                        ./Slot_1/Next ~ ./Start;
                        Btn2_2 : /*/Modules/FvWidgetsL/FButtonL
                        {
                            Text = "Button 2_2";
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        Slot_2 : /*/Modules/ContainerModL/FVLayoutLSlot;
                        ./Slot_2/SCp ~ ./Btn2_2/Cp;
                        ./Slot_2/Next ~ ./Slot_1/Prev;
                        ./End ~ ./Slot_2/Prev;
                    }
                    Slot_1 : /*/Modules/ContainerModL/FHLayoutLSlot;
                    ./Slot_1/SCp ~ ./VBox2/Cp;
                    ./Slot_1/Next ~ Start;
                    $ # " ==== Slot 2 ====";
                    Btn3 : /*/Modules/FvWidgetsL/FButtonL
                    {
                        Text = "Button_3";
                        BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                    Slot_2 : /*/Modules/ContainerModL/FHLayoutLSlot;
                    ./Slot_2/SCp ~ ./Btn3/Cp;
                    ./Slot_2/Next ~ ./Slot_1/Prev;
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
}
