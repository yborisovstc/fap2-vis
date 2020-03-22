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
        $ + /FvWidgets/FWidget;
        $ + /FvWidgets/FButton;
        $ + /ContainerMdl/VLayout;
        $ + /ContainerMdl/HLayout;
        $ + /ContainerMdl/Alignment;
    }
    Launcher : AVDesLauncher;
    Launcher < Path = /testroot/Test;
    Test : /testroot/Modules/DesComps/Des
    {
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
                    VBox : /*/Modules/ContainerMdl/VLayout
                    {
                        Padding = 20;
                        AlcW < Value = "SI 220";
                        AlcH < Value = "SI 330";
                        Slot_1 : AVSlot;
                        Slot_1 < HBox : /*/Modules/ContainerMdl/HLayout
                        {
                            Padding = 2;
                            Slot_1 : AVSlot;
                            ./Slot_1 < Btn1 : /*/Modules/FvWidgets/FButton
                            {
                                Text = "Button 1";
                                BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                                FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            }
                            Slot_2 : AVSlot;
                            ./Slot_2 < Btn2 : /*/Modules/FvWidgets/FButton
                            {
                                Text = "Button 2 test";
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
                        Slot_2 < Btn1 : /*/Modules/FvWidgets/FButton
                        {
                            Text = "Button 1";
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                        }
                        Slot_3 : AVSlot;
                        Slot_3 < Align : /*/Modules/ContainerMdl/Alignment
                        {
                            ./Slot_1 < Btn2 : /*/Modules/FvWidgets/FButton
                            {
                                Text = "Button 2";
                                BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                                FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            }
                        }
                    }
                }
            }
        }
        EnvWidth : AStatec;
        EnvHeight : AStatec;
        Title : AStatec;
        EnvWidth ~ ./Env/Window/Inp_W;
        EnvHeight ~ ./Env/Window/Inp_H;
        Title ~ ./Env/Window/Inp_Title;
        $ # " Initialization";
        EnvWidth < Value = "SI 640";
        EnvHeight < Value = "SI 480";
        Title < Value = "SS Title";
    }
}
